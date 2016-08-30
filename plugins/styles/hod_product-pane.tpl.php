<?php
drupal_add_css(drupal_get_path('module','hod_product') .'/plugins/styles/hod_product.css', array('group' => CSS_DEFAULT, 'every_page' => TRUE)); ?>
<div class="hod1">
<?php if (isset($content->title)): ?>

  <h2 class="pane-title <?php print (isset($settings['top_color'])) ? $settings['top_color'] : 'blue'; ?>">
  <?php print $content->title; ?></h2>

<?php endif ?>

<div class="hod2">

<?php print render($content->content); ?>

</div>
</div>
