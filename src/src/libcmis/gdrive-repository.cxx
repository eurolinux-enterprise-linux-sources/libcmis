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
 * Copyright (C) 2013 Cao Cuong Ngo <cao.cuong.ngo@gmail.com>
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

#include "gdrive-repository.hxx"

GdriveRepository::GdriveRepository( ) :
    Repository( )
{ 
    m_id = "GoogleDrive";
    m_name = "Google Drive";
    m_description = "Google Drive repository";
    m_productName = "Google Drive";
    m_productVersion = "v2";
    m_rootId = "root";
 
    m_capabilities[ ACL ] = "discover";
    m_capabilities[ AllVersionsSearchable ] = "true";
    m_capabilities[ Changes ] = "all";
    m_capabilities[ GetDescendants ] = "true";
    m_capabilities[ GetFolderTree ] = "true";
    m_capabilities[ OrderBy ] = "custom";
    m_capabilities[ Multifiling ] = "true";
    m_capabilities[ PWCSearchable ] = "true";
    m_capabilities[ PWCUpdatable ] = "true";
    m_capabilities[ Query ] = "bothcombined";
    m_capabilities[ Renditions ] = "read";
    m_capabilities[ Unfiling ] = "false";
    m_capabilities[ VersionSpecificFiling ] = "false";
    m_capabilities[ Join ] = "none";
}
