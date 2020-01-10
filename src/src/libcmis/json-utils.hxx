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

#ifndef _JSON_UTILS_HXX_
#define _JSON_UTILS_HXX_

#include <string>
#include <map>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "property.hxx"
#include "exception.hxx"

class Json
{
    public :
        typedef std::map< std::string, Json > JsonObject ;
        typedef std::vector< Json > JsonVector ;
        enum Type { json_null, json_bool, json_double, json_int, json_object, 
            json_array, json_string, json_datetime } ;

        Json( );
        Json( const Json& copy );
        Json( const char *str );
        Json( const libcmis::PropertyPtr& property );
        Json( const libcmis::PropertyPtrMap& properties );
        Json( const JsonVector& arr );
        Json( const JsonObject& obj );

        ~Json( ) ;

        Json operator[]( std::string key ) const 
            throw( libcmis::Exception );

        Json& operator=( const Json& rhs ) ;

        void swap( Json& other ) ;

        void add( const Json& json)
            throw( libcmis::Exception );
        
        void add( const std::string& key, const Json& json)
            throw( libcmis::Exception );

        static Json parse( const std::string& str )
            throw( libcmis::Exception );
        
        std::string toString( ) const;
        Type getDataType( ) const ;
        std::string getStrType( ) const ;

        JsonObject getObjects();
        JsonVector getList();

        boost::property_tree::ptree getTree( ) const{ return m_tJson; }
    private :
        Json( boost::property_tree::ptree tJson ) ;
        boost::property_tree::ptree m_tJson ;
        Type m_type;
        Type parseType( );
} ;

#endif /* _JSON_UTILS_HXX_ */

