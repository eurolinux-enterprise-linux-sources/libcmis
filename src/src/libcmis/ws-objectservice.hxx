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
#ifndef _WS_OBJECTSERVICE_HXX_
#define _WS_OBJECTSERVICE_HXX_

#include <istream>
#include <string>
#include <vector>

#include "base-session.hxx"
#include "document.hxx"
#include "folder.hxx"
#include "object.hxx"
#include "ws-soap.hxx"

class WSSession;

class ObjectService
{
    private:
        WSSession* m_session;
        std::string m_url;

    public:

        ObjectService( WSSession* session );
        ObjectService( const ObjectService& copy );
        ~ObjectService( );

        ObjectService& operator=( const ObjectService& copy );

        libcmis::ObjectPtr getObject( std::string repoId, std::string id ) throw ( libcmis::Exception );

        libcmis::ObjectPtr getObjectByPath( std::string repoId, std::string path ) throw ( libcmis::Exception );
        
        libcmis::ObjectPtr updateProperties(
                std::string repoId,
                std::string objectId,
                const std::map< std::string, libcmis::PropertyPtr > & properties,
                std::string changeToken ) throw ( libcmis::Exception );

        void deleteObject( std::string repoId, std::string id, bool allVersions ) throw ( libcmis::Exception );
        
        std::vector< std::string > deleteTree( std::string repoId, std::string folderId, bool allVersions,
                libcmis::UnfileObjects::Type unfile, bool continueOnFailure ) throw ( libcmis::Exception );

        void move( std::string repoId, std::string objectId, std::string destId, std::string srcId ) throw ( libcmis::Exception );

        boost::shared_ptr< std::istream > getContentStream( std::string repoId, std::string objectId ) throw ( libcmis::Exception );

        void setContentStream( std::string repoId, std::string objectId, bool overwrite, std::string changeToken,
                boost::shared_ptr< std::ostream > stream, std::string contentType, std::string fileName ) throw ( libcmis::Exception );

        libcmis::FolderPtr createFolder( std::string repoId, const std::map< std::string, libcmis::PropertyPtr >& properties,
                std::string folderId ) throw ( libcmis::Exception );

        libcmis::DocumentPtr createDocument( std::string repoId, const std::map< std::string, libcmis::PropertyPtr >& properties,
                std::string folderId, boost::shared_ptr< std::ostream > stream, std::string contentType,
                std::string fileName ) throw ( libcmis::Exception );
};

#endif
