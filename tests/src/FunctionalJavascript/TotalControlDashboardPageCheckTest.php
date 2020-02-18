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
    'total_control',
  ];

  /**
   * A user with the 'have total control' permission.
   *
   * @var \Drupal\user\UserInterface
   */
  protected $webUser;

  /**
   * The profile to install as a basis for testing.
   *
   * Using the standard profile to test user picture display in comments.
   *
   * @var string
   */
  protected $profile = 'standard';

   /**
   * {@inheritdoc}
   */
  protected $defaultTheme = 'bartik';

  /**
   * Tests Total Control Dashboard Page Check.
   */
  public function testTotalControlDashboardPageCheck() {

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

    $this->webUser = $this->drupalCreateUser($permissions);
    $this->drupalLogin($this->webUser);

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

}
