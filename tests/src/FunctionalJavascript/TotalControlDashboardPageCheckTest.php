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
    'user',
    'filter',
    'text',
    'toolbar',
    'contextual',
    'menu_link_content',
    'field',
    'field_ui',
    'filter',
    'history',
    'dynamic_page_cache',
    'page_cache',
    'menu_ui',
    'node',
    'ctools',
    'ctools_block',
    'ctools_views',
    'layout_discovery',
    'block',
    'block_content',
    'contextual',
    'views_ui',
    'views',
    'page_manager',
    'page_manager_ui',
    'panels',
    'taxonomy',
    'comment',
    'total_control',
  ];

  /**
   * The profile used during tests.
   *
   * This purposefully uses the standard profile.
   *
   * @var string
   */
  public $profile = 'standard';

  /**
   * A user with the 'have total control' permission.
   *
   * @var \Drupal\user\UserInterface
   */
  protected $webUser;

  /**
   * {@inheritdoc}
   */
  protected function setUp() {
    parent::setUp();

    $permissions = [
      'administer actions',
      'administer content types',
      'administer content',
      'administer comments',
      'administer vocabularies and terms',
      'administer users',
      'access comments',
      'access content',
      'access toolbar',
      'view the administration theme',
      'have total control',
      'administer pages',
    ];

    $this->webUser = $this->drupalCreateUser($permissions);
    $this->drupalLogin($this->webUser);
  }

  /**
   * Tests Total Control Dashboard Page Check.
   */
  public function testTotalControlDashboardPageCheck() {
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
