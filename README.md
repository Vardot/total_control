# SUMMARY - Total Control

The Total Control Admin Dashboard creates a default panel page with useful
administration tools. Its purpose is to create a central location from which
a Drupal site can be properly cared for. Several overview panes are included
for site stats and quick reference. Several administration panes are provided
with quick links to content types, menus, taxonomy, and other scattered
locations of important Drupal site administration. Several views panes
are also provided as well as full-page comprehensive versions of the views
with bulk operations. Each views panel pane is customizable via it's pane
settings, or override the default views provided to suit your own needs.

[![](https://www.drupal.org/files/images/dashboard_1.png)](http://drupal.org/project/total_control)

For a full description of the module, visit the project page:
  http://drupal.org/project/total_control


## REQUIREMENTS

* Chaos Tools
* Panels
* Views
* Page Manager


## INSTALLATION

Install this module as usual, see

https://www.drupal.org/docs/extending-drupal/installing-modules


## CONFIGURATION

Configure user permissions in Administer >> People >> Permissions

  * have total control
    Users in roles with the "Have total control" permission will see
    the administration dashboard and all included view pages.

## CUSTOMIZATION

To override the default lists on the dashboard, you have two options:

  1. Edit the settings on the panel pane:
     * Use the cog wheel at the top right of the panel display
     * (or visit Admin > Structure > Pages)
     * Configure the pane in question (via cog wheel at top right of pane)

  2. Override the default views provided with the total_control module:
     * use the cog wheel at the top right of the view display
     * (or visit Admin > Structure > Views)


## SUPPORT

Please use the issue queue to report bugs or request support:

http://drupal.org/project/issues/total_control
