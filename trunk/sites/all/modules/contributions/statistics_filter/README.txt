The statistics_filter module allows customizable filtering of hits
from particular user roles or from crawlers. 

=============
 Benefits
=============
For sites with light traffic (i.e., most sites), a large percentage
of the gross hits recorded by statistics.module are either from the
site administrator or from search engines. Filtering out these hits
makes the collected statistics more accurately reflect traffic from
real, human visitors.

===============
 Installation
===============
Place the statistics_filter folder in the modules directory of your 
Drupal installation, and enable it in the administration tools. Then
use the statistics_filter settings to choose what hits to ignore.

To ignore search engines, you must install and enable the browscap module.

Note that statistics_filter is an extension to the statistics module, which must be enabled.

=============
 Credits
=============
Mike Ryan (http://drupal.org/user/4420) is the author of this module. 
Much of the code is based directly on the core statistics module.

Initial conversion to Drupal 6 by Frank Ralf 
(http://drupal.org/user/216048), co-maintainer. 
