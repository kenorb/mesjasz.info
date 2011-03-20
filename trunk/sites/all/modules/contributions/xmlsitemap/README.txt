$Id: README.txt,v 1.3 2008/05/20 17:48:46 darrenoh Exp $
XML Sitemap Module
Author: Matthew Loar <matthew at loar dot name>
This module was originally written as part of Google Summer of Code 2005.

DESCRIPTION
-----------
The XML Sitemap module creates an XML site map at
http://www.example.com/?q=sitemap.xml that conforms to the sitemaps.org
specification. It provides many options for customizing the data reported in the
site map.

INSTALLATION
------------
See INSTALL.txt in this directory.

SPECIALIZED SITE MAPS
---------------------
You can create specialized site maps using Views. To do so, enable both Views
and XML Sitemap: Node and create a new view. In the Page settings, choose XML
Sitemap: Sitemap as the view type for a regular site map or XML Sitemap: News
for a Google News site map.

KNOWN ISSUES
------------
Users who have not enabled clean URLs have reported receiving an
"Unsupported file format" error from Google. The solution is to replace
"?q=sitemap.xml" at the end of the submission URL with
"index.php?q=sitemap.xml". Submission URLs for each search engine can be
configured at http://www.example.com/?q=admin/settings/xmlsitemap/engines.

MORE INFORMATION
----------------
The Sitemap protocol: http://sitemaps.org.

Search engines:
http://www.google.com/webmasters/sitemap (Google)
http://developer.yahoo.com/search/siteexplorer/V1/ping.html (Yahoo!)
http://webmaster.live.com/ (Windows Live)

