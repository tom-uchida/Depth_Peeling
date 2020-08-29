#pragma once

#include <kvs/Version> //KVS2

#include <kvs/Scene> //KVS2

#include <kvs/InitializeEventListener>
#include <kvs/KeyPressEventListener>
#include <kvs/Key>
#include <kvs/glut/Screen>
#include <kvs/ColorImage>
#include <kvs/Camera>

#include "Input.h"

class KeyPressEvent : public kvs::KeyPressEventListener
{
    void update( kvs::KeyEvent* e )
    {
        switch ( e->key() )
        {
            case kvs::Key::s:
            {
                std::cerr << "\n** Snapshot (BMP)" << std::endl;
                kvs::ColorImage snapshot_image = static_cast<kvs::glut::Screen*>( screen() )->scene()->camera()->snapshot();

                char str[256];
                extern size_t nPeels;
                snprintf(str, sizeof str, "%zu", nPeels);
                std::string npeel = str;

                snapshot_image.write( "./IMAGE_DATA/Layer" + npeel + ".bmp" );
            }

            default:
                break;
        } // end switch
    }
};