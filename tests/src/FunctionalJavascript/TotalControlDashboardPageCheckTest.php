<?php

namespace Drupal\Tests\total_control\FunctionalJavascript;

use Drupal\FunctionalJavascriptTests\WebDriverTestBase;
use Drupal\Core\StringTranslation\StringTranslationTrait;

/**
 * Tests the UI for Total Control Dashboard Page.
 *
 * @group total_control
 */
class TotalControlDashboardPageCheckTest extends WebDriverTestBase {

  use StringTranslationTrait;

  /**
   * A user with the 'have total control' permission.
   *
   * @var \Drupal\user\UserInterface
   */
  protected $dashboardAdminUser;

  /**
   * {@inheritdoc}
   */
  protected $profile = 'standard';

  /**
   * {@inheritdoc}
   */
  protected $defaultTheme = 'olivero';

  /**
   * {@inheritdoc}
   */
  // phpcs:ignore
  protected $strictConfigSchema = FALSE;

  /**
   * Modules to install.
   *
   * @var array
   */
  public static $modules = [
    'system',
    'node',
    'user',
    'comment',
    'taxonomy',
    'filter',
    'text',
    'link',
    'toolbar',
    'block',
    'block_content',
    'views',
    'views_ui',
    'contextual',
    'ctools',
    'ctools_block',
    'ctools_views',
    'layout_discovery',
    'panels',
    'page_manager',
    'page_manager_ui',
    'total_control',
  ];

  /**
   * {@inheritdoc}
   */
  protected function setUp(): void {
    parent::setUp();

    // Insall the Claro admin theme.
    $this->container->get('theme_installer')->install(['claro']);

    // Set the Claro theme as the default admin theme.
    $this->config('system.theme')->set('admin', 'claro')->save();

    $permissions = [
      'administer content types',
      'administer menu',
      'administer comments',
      'administer taxonomy',
      'administer users',
      'administer pages',
      'administer blocks',
      'administer views',
      'administer panels layouts',
      'administer panels styles',
      'administer pane access',
      'administer advanced pane settings',
      'access administration pages',
      'access user profiles',
      'access content',
      'access comments',
      'access toolbar',
      'access contextual links',
      'view the administration theme',
      'view pane admin links',
      'use panels caching features',
      'use panels dashboard',
      'use panels locks',
      'use ipe with page manager',
      'have total control',
    ];

    $this->dashboardAdminUser = $this->drupalCreateUser($permissions);

  }

  /**
   * Tests Total Control Dashboard Page Check.
   */
  public function testTotalControlDashboardPageCheck() {

    $this->drupalLogin($this->dashboardAdminUser);

    $this->drupalGet('admin');
    $this->assertSession()->waitForElementVisible('css', '#toolbar-link-page_manager-page_view_total_control_dashboard_total_control_dashboard-http_status_code-0');
    $dashboard_toolbar_text = $this->t('Dashboard');
    $this->assertSession()->pageTextContains($dashboard_toolbar_text);
    $this->clickLink($dashboard_toolbar_text);

    $this->drupalGet('admin/dashboard');
    $this->assertSession()->waitForElementVisible('css', '.block.block-total-control.block-total-control-dashboard');
    $this->assertSession()->pageTextContains($this->t('Welcome to your administrative dashboard.'));
    $this->assertSession()->pageTextContains($this->t('Create Content'));
    $this->assertSession()->pageTextContains($this->t('Content Overview'));
    $this->assertSession()->pageTextContains($this->t('New User Accounts'));
    $this->assertSession()->pageTextContains($this->t('New Content'));
    $this->assertSession()->pageTextContains($this->t('Administer Menus'));
    $this->assertSession()->pageTextContains($this->t('Administer Content Types'));
    $this->assertSession()->pageTextContains($this->t('Administer Taxonomy'));
    $this->assertSession()->pageTextContains($this->t('Administer Panel Pages'));
  }

  /**
   * Test check on Clicking on Edit this panel to add more blocks.
   */
  public function testCheckOnClickingEditThisPanelToAddMoreBlocksLink() {

    $this->drupalLogin($this->dashboardAdminUser);

    $this->drupalGet('admin/dashboard');
    $this->assertSession()->pageTextContains('Dashboard');
    $this->assertSession()->pageTextContains($this->t('Welcome to your administrative dashboard. Edit this panel to add more blocks here, or configure those provided by default.'));

    $this->clickLink('Edit this panel');
    $this->assertSession()->pageTextContains('Variants');
    $this->assertSession()->pageTextContains('Top');
    $this->assertSession()->pageTextContains('First above');
    $this->assertSession()->pageTextContains('Second above');

    $page = $this->getSession()->getPage();
    $page->findButton('Update and save')->click();
    $this->assertSession()->pageTextContains('The page Total Control dashboard has been updated.');

  }

}
