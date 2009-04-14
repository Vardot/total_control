/* $Id$ */

-- SUMMARY --

The Total Control Administration Dashboard provides a default panel page with 
useful administration view panes out of the box.  Each View pane
is customizable via pane config.  

For a full description of the module, visit the project page:
  http://drupal.org/project/total_control

To submit bug reports and feature suggestions, or to track changes:
  http://drupal.org/project/issues/total_control


-- REQUIREMENTS --

Panels 3. 
Views 2.


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/70151 for further information.

* TODO better documentation here: version 1.1beta requires that you import a panel page. 
  Copy / paste the contents of total_control_panes at admin/build/pages


-- CONFIGURATION --

* Configure user permissions in Administer >> User management >> Access control
  >> total_control module:

  - have total control

    Users in roles with the "have total control" permission will see
    the administration dashboard and all associated view pages.
    

-- CUSTOMIZATION --

* To override the default views on the dashboard, you have two options:
  
  - edit the settings on page config:
  
    TODO: instructions
  
  - override the default views provided with the total_control module:
  
    TODO: instructions