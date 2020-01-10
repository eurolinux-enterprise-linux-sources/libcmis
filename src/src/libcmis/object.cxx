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

#include "object.hxx"
#include "session.hxx"
#include "xml-utils.hxx"

using namespace std;

namespace libcmis
{
    Object::Object( Session* session ) :
        m_session( session ),
        m_typeDescription( ),
        m_refreshTimestamp( 0 ),
        m_typeId( ),
        m_properties( ),
        m_allowableActions( )
    {
    }

    Object::Object( Session* session, xmlNodePtr node ) :
        m_session( session ),
        m_typeDescription( ),
        m_refreshTimestamp( 0 ),
        m_typeId( ),
        m_properties( ),
        m_allowableActions( )
    {
        initializeFromNode( node );
    }

    Object::Object( const Object& copy ) :
        m_session( copy.m_session ),
        m_typeDescription( copy.m_typeDescription ),
        m_refreshTimestamp( copy.m_refreshTimestamp ),
        m_typeId( copy.m_typeId ),
        m_properties( copy.m_properties ),
        m_allowableActions( copy.m_allowableActions )
    {
    }

    Object& Object::operator=( const Object& copy )
    {
        if ( this != &copy )
        {
            m_session = copy.m_session;
            m_typeDescription = copy.m_typeDescription;
            m_refreshTimestamp = copy.m_refreshTimestamp;
            m_typeId = copy.m_typeId;
            m_properties = copy.m_properties;
            m_allowableActions = copy.m_allowableActions;
        }

        return *this;
    }

    void Object::initializeFromNode( xmlNodePtr node )
    {
        // Even if node is NULL we'll have an empty doc, so no need
        // to worry about it.
        xmlDocPtr doc = wrapInDoc( node );
        xmlXPathContextPtr xpathCtx = xmlXPathNewContext( doc );

        libcmis::registerNamespaces( xpathCtx );

        if ( NULL != xpathCtx )
        {
            // Get the allowableActions
            xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression( BAD_CAST( "//cmis:allowableActions" ), xpathCtx );
            if ( xpathObj && xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0 )
            {
                xmlNodePtr actionsNode = xpathObj->nodesetval->nodeTab[0];
                m_allowableActions.reset( new libcmis::AllowableActions( actionsNode ) );
            }
            xmlXPathFreeObject( xpathObj );

            // First get the type id as it will give us the property definitions
            string typeIdReq( "//cmis:propertyId[@propertyDefinitionId='cmis:objectTypeId']/cmis:value/text()" );
            m_typeId = libcmis::getXPathValue( xpathCtx, typeIdReq );

            string propertiesReq( "//cmis:properties/*" );
            xpathObj = xmlXPathEvalExpression( BAD_CAST( propertiesReq.c_str() ), xpathCtx );
            if ( NULL != xpathObj && NULL != xpathObj->nodesetval )
            {
                int size = xpathObj->nodesetval->nodeNr;
                for ( int i = 0; i < size; i++ )
                {
                    xmlNodePtr propertyNode = xpathObj->nodesetval->nodeTab[i];
                    libcmis::PropertyPtr property = libcmis::parseProperty( propertyNode, getTypeDescription( ) );
                    if ( property != NULL )
                        m_properties[ property->getPropertyType( )->getId() ] = property;
                }
            }
            xmlXPathFreeObject( xpathObj );
        }

        xmlXPathFreeContext( xpathCtx );
        xmlFreeDoc( doc );

        m_refreshTimestamp = time( NULL );
    } 

    string Object::getId( )
    {
        string name;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:objectId" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            name = it->second->getStrings( ).front( );
        return name;
    }

    string Object::getName( )
    {
        string name;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:name" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            name = it->second->getStrings( ).front( );
        return name;
    }

    vector< string > Object::getPaths( )
    {
        return vector< string > ( );
    }

    string Object::getBaseType( )
    {
        string value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:baseTypeId" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            value = it->second->getStrings( ).front( );
        return value;
    }

    string Object::getType( )
    {
        string value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:objectTypeId" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            value = it->second->getStrings( ).front( );

        if ( value.empty( ) )
            value = m_typeId;
        return value;
    }

    string Object::getCreatedBy( )
    {
        string value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:createdBy" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            value = it->second->getStrings( ).front( );
        return value;
    }

    boost::posix_time::ptime Object::getCreationDate( )
    {
        boost::posix_time::ptime value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:creationDate" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getDateTimes( ).empty( ) )
            value = it->second->getDateTimes( ).front( );
        return value;
    }

    string Object::getLastModifiedBy( )
    {
        string value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:lastModifiedBy" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            value = it->second->getStrings( ).front( );
        return value;
    }

    boost::posix_time::ptime Object::getLastModificationDate( )
    {
        boost::posix_time::ptime value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:lastModificationDate" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getDateTimes( ).empty( ) )
            value = it->second->getDateTimes( ).front( );
        return value;
    }

    bool Object::isImmutable( )
    {
        bool value = false;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:isImmutable" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getBools( ).empty( ) )
            value = it->second->getBools( ).front( );
        return value;
    }

    string Object::getChangeToken( )
    {
        string value;
        map< string, libcmis::PropertyPtr >::const_iterator it = getProperties( ).find( string( "cmis:changeToken" ) );
        if ( it != getProperties( ).end( ) && it->second != NULL && !it->second->getStrings( ).empty( ) )
            value = it->second->getStrings( ).front( );
        return value;
    }

    map< string, libcmis::PropertyPtr >& Object::getProperties( )
    {
        return m_properties;
    }

    libcmis::ObjectTypePtr Object::getTypeDescription( )
    {
        if ( !m_typeDescription.get( ) && m_session != NULL )
            m_typeDescription = m_session->getType( getType( ) );

        return m_typeDescription;
    }

    string Object::toString( )
    {
        stringstream buf;

        buf << "Id: " << getId() << endl;
        buf << "Name: " << getName() << endl;
        buf << "Type: " << getType() << endl;
        buf << "Base type: " << getBaseType() << endl;
        buf << "Created on " << boost::posix_time::to_simple_string( getCreationDate() )
            << " by " << getCreatedBy() << endl;
        buf << "Last modified on " << boost::posix_time::to_simple_string( getLastModificationDate() )
            << " by " << getLastModifiedBy() << endl;
        buf << "Change token: " << getChangeToken() << endl;

        // Write remaining properties
        static const char* skippedProps[] = {
            "cmis:name", "cmis:baseTypeId", "cmis:objectTypeId", "cmis:createdBy",
            "cmis:creationDate", "cmis:lastModifiedBy", "cmis:lastModificationDate",
            "cmis::changeToken"
        };
        int skippedCount = sizeof( skippedProps ) / sizeof( char* );

        for ( map< string, libcmis::PropertyPtr >::iterator it = getProperties( ).begin();
                it != getProperties( ).end( ); ++it )
        {
            string propId = it->first;
            bool toSkip = false;
            for ( int i = 0; i < skippedCount && !toSkip; ++i )
            {
                toSkip = propId == skippedProps[i];
            }

            if ( !toSkip )
            {
                libcmis::PropertyPtr prop = it->second;
                if ( prop != NULL && prop->getPropertyType( ) != NULL )
                {
                    buf << prop->getPropertyType( )->getDisplayName( ) << "( " << prop->getPropertyType()->getId( ) << " ): " << endl;
                    vector< string > strValues = prop->getStrings( );
                    for ( vector< string >::iterator valueIt = strValues.begin( );
                          valueIt != strValues.end( ); ++valueIt )
                    {
                        buf << "\t" << *valueIt << endl; 
                    }
                }
            }
        }

        return buf.str();
    }

    void Object::toXml( xmlTextWriterPtr writer )
    {
        // Output the properties
        xmlTextWriterStartElement( writer, BAD_CAST( "cmis:properties" ) );
        for ( map< string, libcmis::PropertyPtr >::iterator it = getProperties( ).begin( );
                it != getProperties( ).end( ); ++it )
        {
            it->second->toXml( writer );
        }
        xmlTextWriterEndElement( writer ); // cmis:properties
    }
}
