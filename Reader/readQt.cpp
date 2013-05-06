    //
//  readJpeg.cpp
//  PowiterOsX
//
//  Created by Alexandre on 1/15/13.
//  Copyright (c) 2013 Alexandre. All rights reserved.
//

#include "Reader/readQt.h"
#include "Gui/GLViewer.h"
#include <QtGui/QImage>
#include <QtGui/QColor>
#include "Reader/Reader.h"
#include "Gui/node_ui.h"
#include "Core/lutclasses.h"
using namespace std;
ReadJPEG::ReadJPEG(const QStringList& file_list,Reader* op,ViewerGL* ui_context) : Read(file_list,op,ui_context){
    _lut=Lut::getLut(Lut::VIEWER);
    _lut->validate();
    _img= new QImage;
}

ReadJPEG::~ReadJPEG(){
    delete _img;
}
void ReadJPEG::engine(int y,int offset,int range,ChannelMask channels,Row* out){
    uchar* buffer;
    int h = op->getInfo()->getFull_size_format().h();
    int Y = h - y - 1;
    buffer = _img->scanLine(Y);
    float* r = out->writable(Channel_red) + offset;
	float* g = out->writable(Channel_green) + offset;
	float* b = out->writable(Channel_blue) + offset;
	float* a = channels &Channel_alpha ? out->writable(Channel_alpha) + offset : NULL;
    if(autoAlpha() && a==NULL){
        out->turn_on(Channel_alpha);
        a =out->writable(Channel_alpha)+offset;
    }
    int depth = 4; 
	from_byte(r,g,b,buffer + offset * depth,channels & Channel_alpha,(range-offset),depth,a);
     
}
bool ReadJPEG::supports_stereo(){
    return false;
}
void ReadJPEG::open(){
    QString filename = files[current_frame];
    if(!_img->load(filename)){
        cout << "ERROR reading image file : " << qPrintable(files[current_frame]) << endl;
    }
    if(_img->format() == QImage::Format_Invalid){
        cout << "Couldn't load this image format" << endl;
        return;
    }
    int width = _img->width();
    int height= _img->height();
    double aspect = _img->dotsPerMeterX()/ _img->dotsPerMeterY();
    
	bool rgb=true;
	int ydirection = -1;
	ChannelMask mask;
    op->getInfo()->rgbMode(true);
    
    if(_autoCreateAlpha){
        
        op->getInfo()->set_channels(Mask_RGBA);
		mask = Mask_RGBA;

    }else{
        if(_img->format()==QImage::Format_ARGB32 || _img->format()==QImage::Format_ARGB32_Premultiplied
           || _img->format()==QImage::Format_ARGB4444_Premultiplied ||
           _img->format()==QImage::Format_ARGB6666_Premultiplied ||
           _img->format()==QImage::Format_ARGB8555_Premultiplied ||
           _img->format()==QImage::Format_ARGB8565_Premultiplied){
            
            if(_img->format()== QImage::Format_ARGB32_Premultiplied
               || _img->format()== QImage::Format_ARGB4444_Premultiplied
               || _img->format()== QImage::Format_ARGB6666_Premultiplied
               || _img->format()== QImage::Format_ARGB8555_Premultiplied
               || _img->format()== QImage::Format_ARGB8565_Premultiplied){
                _premult=true;
                
            }
            op->getInfo()->set_channels(Mask_RGBA);
            mask = Mask_RGBA;
        }
        else{
            op->getInfo()->set_channels(Mask_RGB);
			mask = Mask_RGB;

        }
    }

    DisplayFormat format(0,0,width,height,"",aspect);
    op->getInfo()->set_full_size_format(format);
    op->getInfo()->set(0, 0, width, height);
    op->getInfo()->setYdirection(-1);

    setReaderInfo(format,format,filename,mask,ydirection,rgb,current_frame,firstFrame(),lastFrame());
    
}
void ReadJPEG::make_preview(){
    op->setPreview(_img);
    op->getNodeUi()->updatePreviewImageForReader();
}
