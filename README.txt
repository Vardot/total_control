/* $Id$ */

-- SUMMARY --

The Total Control Admin Dashboard creates a default panel page with useful 
administration panes right out of the box. Several overview panes are included 
for site stats and quick reference. Administration view panes are provided with 
'more' links to more comprehensive versions of the views. Each View pane is 
customizable via pane config, or override the defaults provided to suit your 
own needs.  

For a full description of the module, visit the project page:
  http://drupal.org/project/total_control

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/total_control


-- REQUIREMENTS --

Panels 3. 
Views 2.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* TODO better documentation here: version 1.1-beta requires that you import a panel page. 
  Copy / paste the contents of total_control_page.inc to import screen at admin/build/pages


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Access control
  >> total_control module:

  - have total control

    Users in roles with the "have total control" permission will see
    the administration dashboard and all included view pages.
    

-- CUSTOMIZATION --

* To override the default views on the dashboard, you have two options:
  
  - edit the settings on the panel pane:
  
    TODO: instructions
  
  - override the default views provided with the total_control module:
  
    TODO: instructions