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
#include <sstream>

#include "atom-document.hxx"
#include "atom-folder.hxx"
#include "atom-session.hxx"
#include "xml-utils.hxx"

using namespace std;

namespace
{
}

AtomFolder::AtomFolder( AtomPubSession* session, xmlNodePtr entryNd ) :
    libcmis::Object( session ),
    libcmis::Folder( session ),
    AtomObject( session )
{
    xmlDocPtr doc = libcmis::wrapInDoc( entryNd );
    refreshImpl( doc );
    xmlFreeDoc( doc );
}


AtomFolder::~AtomFolder( )
{
}

vector< libcmis::ObjectPtr > AtomFolder::getChildren( ) throw ( libcmis::Exception )
{
    AtomLink* childrenLink = getLink( "down", "application/atom+xml;type=feed" );

    if ( ( NULL == childrenLink ) || ( getAllowableActions( ).get() &&
                !getAllowableActions()->isAllowed( libcmis::ObjectAction::GetChildren ) ) )
        throw libcmis::Exception( string( "GetChildren not allowed on node " ) + getId() );

    vector< libcmis::ObjectPtr > children;

    string pageUrl = childrenLink->getHref( );

    bool hasNext = true;
    while ( hasNext )
    {
        string buf;
        try
        {
            buf = getSession()->httpGetRequest( pageUrl )->getStream( )->str( );
        }
        catch ( const CurlException& e )
        {
            throw e.getCmisException( );
        }

        xmlDocPtr doc = xmlReadMemory( buf.c_str(), buf.size(), pageUrl.c_str(), NULL, 0 );
        if ( NULL != doc )
        {
            xmlXPathContextPtr xpathCtx = xmlXPathNewContext( doc );
            libcmis::registerNamespaces( xpathCtx );
            if ( NULL != xpathCtx )
            {
                // Check if there is a next link to handled paged results
                const string& nextReq( "/atom:feed/atom:link[@rel='next']/attribute::href" );
                string nextHref = libcmis::getXPathValue( xpathCtx, nextReq );
                hasNext = !nextHref.empty( );
                if ( hasNext )
                    pageUrl = nextHref;

                // Get the page entries
                const string& entriesReq( "//atom:entry" );
                xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression( BAD_CAST( entriesReq.c_str() ), xpathCtx );

                if ( NULL != xpathObj && NULL != xpathObj->nodesetval )
                {
                    int size = xpathObj->nodesetval->nodeNr;
                    for ( int i = 0; i < size; i++ )
                    {
                        xmlNodePtr node = xpathObj->nodesetval->nodeTab[i];
                        xmlDocPtr entryDoc = libcmis::wrapInDoc( node );
                        libcmis::ObjectPtr cmisObject = getSession()->createObjectFromEntryDoc( entryDoc );

                        if ( cmisObject.get() )
                            children.push_back( cmisObject );
                        xmlFreeDoc( entryDoc );
                    }
                }

                xmlXPathFreeObject( xpathObj );
            }

            xmlXPathFreeContext( xpathCtx );
        }
        else
        {
            throw libcmis::Exception( "Failed to parse folder infos" );
        }
        xmlFreeDoc( doc );
    }

    return children;
}

libcmis::FolderPtr AtomFolder::createFolder( const map< string, libcmis::PropertyPtr >& properties )
    throw( libcmis::Exception )
{
    AtomLink* childrenLink = getLink( "down", "application/atom+xml;type=feed" );

    if ( ( NULL == childrenLink ) || ( getAllowableActions( ).get() &&
                !getAllowableActions()->isAllowed( libcmis::ObjectAction::CreateFolder ) ) )
        throw libcmis::Exception( string( "CreateFolder not allowed on folder " ) + getId() );

    xmlBufferPtr buf = xmlBufferCreate( );
    xmlTextWriterPtr writer = xmlNewTextWriterMemory( buf, 0 );

    xmlTextWriterStartDocument( writer, NULL, NULL, NULL );

    // Copy and remove the readonly properties before serializing
    boost::shared_ptr< ostream > stream;
    AtomObject::writeAtomEntry( writer, properties, stream, string( ) );

    xmlTextWriterEndDocument( writer );
    string str( ( const char * )xmlBufferContent( buf ) );
    istringstream is( str );

    xmlFreeTextWriter( writer );
    xmlBufferFree( buf );

    libcmis::HttpResponsePtr response;
    try
    {
        response = getSession( )->httpPostRequest( childrenLink->getHref( ), is, "application/atom+xml;type=entry" );
    }
    catch ( const CurlException& e )
    {
        throw e.getCmisException( );
    }

    string respBuf = response->getStream( )->str( );
    xmlDocPtr doc = xmlReadMemory( respBuf.c_str(), respBuf.size(), getInfosUrl().c_str(), NULL, 0 );
    if ( NULL == doc )
        throw libcmis::Exception( "Failed to parse object infos" );

    libcmis::ObjectPtr created = getSession( )->createObjectFromEntryDoc( doc );
    xmlFreeDoc( doc );

    libcmis::FolderPtr newFolder = boost::dynamic_pointer_cast< libcmis::Folder >( created );
    if ( !newFolder.get( ) )
        throw libcmis::Exception( string( "Created object is not a folder: " ) + created->getId( ) );

    return newFolder;
}

libcmis::DocumentPtr AtomFolder::createDocument( const map< string, libcmis::PropertyPtr >& properties,
        boost::shared_ptr< ostream > os, string contentType, string ) throw ( libcmis::Exception )
{
    AtomLink* childrenLink = getLink( "down", "application/atom+xml;type=feed" );

    if ( ( NULL == childrenLink ) || ( getAllowableActions( ).get() &&
                !getAllowableActions()->isAllowed( libcmis::ObjectAction::CreateDocument ) ) )
        throw libcmis::Exception( string( "CreateDocument not allowed on folder " ) + getId() );

    xmlBufferPtr buf = xmlBufferCreate( );
    xmlTextWriterPtr writer = xmlNewTextWriterMemory( buf, 0 );

    xmlTextWriterStartDocument( writer, NULL, NULL, NULL );

    AtomObject::writeAtomEntry( writer, properties, os, contentType );

    xmlTextWriterEndDocument( writer );
    string str( ( const char * )xmlBufferContent( buf ) );
    istringstream is( str );

    xmlFreeTextWriter( writer );
    xmlBufferFree( buf );

    libcmis::HttpResponsePtr response;
    try
    {
        response = getSession( )->httpPostRequest( childrenLink->getHref( ), is, "application/atom+xml;type=entry" );
    }
    catch ( const CurlException& e )
    {
        throw e.getCmisException( );
    }

    string respBuf = response->getStream( )->str( );
    xmlDocPtr doc = xmlReadMemory( respBuf.c_str(), respBuf.size(), getInfosUrl().c_str(), NULL, 0 );
    if ( NULL == doc )
        throw libcmis::Exception( "Failed to parse object infos" );

    libcmis::ObjectPtr created = getSession( )->createObjectFromEntryDoc( doc );
    xmlFreeDoc( doc );

    libcmis::DocumentPtr newDocument = boost::dynamic_pointer_cast< libcmis::Document >( created );
    if ( !newDocument.get( ) )
        throw libcmis::Exception( string( "Created object is not a document: " ) + created->getId( ) );

    return newDocument;
}

vector< string > AtomFolder::removeTree( bool allVersions, libcmis::UnfileObjects::Type unfile,
        bool continueOnError ) throw ( libcmis::Exception )
{
    AtomLink* treeLink = getLink( "down", "application/cmistree+xml" );
    if ( NULL == treeLink )
        treeLink = getLink( "http://docs.oasis-open.org/ns/cmis/link/200908/foldertree", "application/cmistree+xml" );

    if ( ( NULL == treeLink ) || ( getAllowableActions( ).get() &&
                !getAllowableActions()->isAllowed( libcmis::ObjectAction::DeleteTree ) ) )
        throw libcmis::Exception( string( "DeleteTree not allowed on folder " ) + getId() );

    try
    {
        string deleteUrl = treeLink->getHref( );
        if ( deleteUrl.find( '?' ) != string::npos )
            deleteUrl += "&";
        else
            deleteUrl += "?";
       
        // Add the all versions parameter 
        string allVersionsStr = "TRUE";
        if ( !allVersions )
            allVersionsStr = "FALSE";
        deleteUrl += "allVersions=" + allVersionsStr;

        // Add the unfileObjects parameter
        string unfileStr;
        switch ( unfile )
        {
            case libcmis::UnfileObjects::Delete:
                unfileStr = "delete";
                break;
            case libcmis::UnfileObjects::DeleteSingleFiled:
                unfileStr = "deletesinglefiled";
                break;
            case libcmis::UnfileObjects::Unfile:
                unfileStr = "unfile";
                break;
            default:
                break;
        }
        deleteUrl += "&unfileObjects=" + unfileStr;

        // Add the continueOnFailure parameter
        string continueOnErrorStr = "TRUE";
        if ( !continueOnError )
            continueOnErrorStr = "FALSE";
        deleteUrl += "&continueOnFailure=" + continueOnErrorStr;

        getSession( )->httpDeleteRequest( deleteUrl );
    }
    catch ( const CurlException& e )
    {
        throw e.getCmisException( );
    }

    // TODO Implement getting the failedIDs using a GET request on the same URL
    return vector< string >( );
}
