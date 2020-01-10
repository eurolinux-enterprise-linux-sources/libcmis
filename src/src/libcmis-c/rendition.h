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

#ifndef _LIBCMIS_RENDITION_H_
#define _LIBCMIS_RENDITION_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "types.h"

void libcmis_rendition_free( libcmis_RenditionPtr rendition );

bool libcmis_rendition_isThumbnail( libcmis_RenditionPtr rendition );

const char* libcmis_rendition_getStreamId( libcmis_RenditionPtr rendition );
const char* libcmis_rendition_getMimeType( libcmis_RenditionPtr rendition );
const char* libcmis_rendition_getKind( libcmis_RenditionPtr rendition );
const char* libcmis_rendition_getUrl( libcmis_RenditionPtr rendition );
const char* libcmis_rendition_getTitle( libcmis_RenditionPtr rendition );
long libcmis_rendition_getLength( libcmis_RenditionPtr rendition );
long libcmis_rendition_getWidth( libcmis_RenditionPtr rendition );
long libcmis_rendition_getHeight( libcmis_RenditionPtr rendition );
const char* libcmis_rendition_getRenditionDocumentId( libcmis_RenditionPtr rendition );

#ifdef __cplusplus
}
#endif

#endif


