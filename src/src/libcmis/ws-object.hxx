/* libcmis
 * Version: MPL 1.1 / GPLv2+ / LGPLv2+
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License or as specified alternatively below. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * Major Contributor(s):
 * Copyright (C) 2011 SUSE <cbosdonnat@suse.com>
 *
 *
 * All Rights Reserved.
 *
 * For minor contributions see the git repository.
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPLv2+"), or
 * the GNU Lesser General Public License Version 2 or later (the "LGPLv2+"),
 * in which case the provisions of the GPLv2+ or the LGPLv2+ are applicable
 * instead of those above.
 */
#ifndef _WS_OBJECT_HXX_
#define _WS_OBJECT_HXX_

#include "folder.hxx"
#include "object.hxx"
#include "ws-session.hxx"

class WSObject : public virtual libcmis::Object
{
    protected:
        WSObject( WSSession* session );

    public:
        WSObject( WSSession* session, xmlNodePtr node );
        WSObject( const WSObject& copy );
        virtual ~WSObject( );

        WSObject& operator=( const WSObject& copy );

        virtual std::vector< libcmis::RenditionPtr > getRenditions( std::string filter ) throw ( libcmis::Exception );
        virtual libcmis::ObjectPtr updateProperties(
                const std::map< std::string, libcmis::PropertyPtr >& properties ) throw ( libcmis::Exception );
        
        virtual void refresh( ) throw ( libcmis::Exception );
        
        virtual void remove( bool allVersions = true ) throw ( libcmis::Exception );

        virtual void move( libcmis::FolderPtr source, libcmis::FolderPtr destination ) throw ( libcmis::Exception );

        WSSession* getSession( );
};

#endif
