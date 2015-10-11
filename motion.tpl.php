<h1>Motion nr. <?php print $entity_id; ?></h1>
<p><?php print $meeting_title; ?>, motie nr. <?php print $id; ?>.</p>
<p>
  <strong><?php print $consideration_opening; ?></strong><br/>
  <?php print $consideration_body; ?><br/>
  <strong><?php print $follow_up_opening; ?></strong><br/>
  <?php print $follow_up_body; ?>
</p>
<p>
  <strong>Toelichting:</strong><br/>
  <?php print $supplement; ?>
</p>
<?php if ($edit_access || $admin_access) : ?>
  <ul>
  <?php if ($edit_access) : ?>
    <li><?php print l('bewerk motie', 'ammo/motion/edit/' . $entity_id, array('query' => drupal_get_destination()))?></li>
  <?php endif; ?>
  <?php if ($admin_access) : ?>
    <li><?php print l('bewerk advies', 'ammo/motion/advice/' . $entity_id, array('query' => drupal_get_destination()))?></li>
  <?php endif; ?>
  </ul>
<?php endif; ?>

