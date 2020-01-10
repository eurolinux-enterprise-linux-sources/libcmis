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

#include "internals.hxx"
#include "property.h"

using namespace std;


libcmis_vector_property_Ptr libcmis_vector_property_create( )
{
    return new ( nothrow ) libcmis_vector_property( );
}


void libcmis_vector_property_free( libcmis_vector_property_Ptr vector )
{
    delete vector;
}


size_t libcmis_vector_property_size( libcmis_vector_property_Ptr vector )
{
    size_t size = 0;
    if ( vector != NULL )
        size = vector->handle.size( );
    return size;
}


libcmis_PropertyPtr libcmis_vector_property_get( libcmis_vector_property_Ptr vector, size_t i )
{
    libcmis_PropertyPtr item = NULL;
    if ( vector != NULL && i < vector->handle.size( ) )
    {
        libcmis::PropertyPtr type = vector->handle[i];
        item = new ( nothrow ) libcmis_property( );
        if ( item )
            item->handle = type;
    }
    return item;
}


void libcmis_vector_property_append( libcmis_vector_property_Ptr vector, libcmis_PropertyPtr item )
{
    if ( vector != NULL &&
            item != NULL && item->handle.get( ) != NULL )
    {
        vector->handle.push_back( item->handle );
    }
}


libcmis_PropertyPtr libcmis_property_create( libcmis_PropertyTypePtr type, const char** strValues, size_t size )
{
    libcmis_PropertyPtr property = NULL;
    if ( type != NULL && type->handle.get( ) != NULL )
    {
        property = new ( nothrow ) libcmis_property( );
        if ( property )
        {
            vector< string > values;
            for ( size_t i = 0; i < size; ++i )
                values.push_back( string( strValues[i] ) );
            libcmis::PropertyPtr prop( new ( nothrow ) libcmis::Property( type->handle, values ) );
            property->handle = prop;
        }
    }

    return property;
}


void libcmis_property_free( libcmis_PropertyPtr property )
{
    delete property;
}


libcmis_PropertyTypePtr libcmis_property_getPropertyType( libcmis_PropertyPtr property )
{
    libcmis_PropertyTypePtr type = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        libcmis::PropertyTypePtr handle = property->handle->getPropertyType( );
        type = new ( nothrow ) libcmis_property_type( );
        if ( type )
            type->handle = handle;
    }
    return type;
}


libcmis_vector_time_Ptr libcmis_property_getDateTimes( libcmis_PropertyPtr property )
{
    libcmis_vector_time_Ptr times = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        vector< boost::posix_time::ptime > handles = property->handle->getDateTimes( );
        times = new ( nothrow ) libcmis_vector_time( );
        if ( times )
            times->handle = handles;
    }
   return times; 
}


libcmis_vector_bool_Ptr libcmis_property_getBools( libcmis_PropertyPtr property )
{
    libcmis_vector_bool_Ptr values = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        vector< bool > handles = property->handle->getBools( );
        values = new ( nothrow ) libcmis_vector_bool( );
        if ( values )
            values->handle = handles;
    }
    return values;
}


libcmis_vector_string_Ptr libcmis_property_getStrings( libcmis_PropertyPtr property )
{
    libcmis_vector_string_Ptr values = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        vector< string > handles = property->handle->getStrings( );
        values = new ( nothrow ) libcmis_vector_string( );
        if ( values ) 
            values->handle = handles;
    }
    return values;
}


libcmis_vector_long_Ptr libcmis_property_getLongs( libcmis_PropertyPtr property )
{
    libcmis_vector_long_Ptr values = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        vector< long > handles = property->handle->getLongs( );
        values = new ( nothrow ) libcmis_vector_long( );
        if ( values )
            values->handle = handles;
    }
    return values;
}


libcmis_vector_double_Ptr libcmis_property_getDoubles( libcmis_PropertyPtr property )
{
    libcmis_vector_double_Ptr values = NULL;
    if ( property != NULL && property->handle.get( ) != NULL )
    {
        vector< double > handles = property->handle->getDoubles( );
        values = new ( nothrow ) libcmis_vector_double( );
        if ( values )
            values->handle = handles;
    }
    return values;
}


void libcmis_property_setValues( libcmis_PropertyPtr property, const char** strValues, size_t size )
{
    if ( property != NULL && property->handle.get() != NULL )
    {
        vector< string > values;
        for ( size_t i = 0; i < size; ++i )
            values.push_back( string( strValues[i] ) );
        property->handle->setValues( values );
    }
}
