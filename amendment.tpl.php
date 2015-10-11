<h1>Amendement nr. <?php print $entity_id; ?></h1>
<?php switch ($type): ?>
<?php case 'specific': ?>
<p>Amendement op het stuk "<?php print $document_title; ?>", hoofdstuk <?php print $chapter; ?>, pagina <?php print $page; ?>, regel <?php print $line; ?>.</p>
<?php switch ($action): ?>
<?php case 'add': ?>
<p><strong>Toe te voegen tekst:</strong><br/><?php print $new_text; ?></p>
<?php break; ?>
<?php case 'delete': ?>
<p><strong>Te verwijderen tekst:</strong><br/><?php print $current_text; ?></p>
<?php break; ?>
<?php case 'replace': ?>
<p><strong>De tekst:</strong><br/><?php print $current_text; ?><br/><strong>vervangen door:</strong><br/><?php print $new_text; ?></p>
<?php break; ?>
<?php endswitch; ?>
<?php break; ?>
<?php case 'general': ?>
<p><strong>Tekst:</strong><br/><?php print $general; ?></p>
<?php break; ?>
<?php endswitch; ?>
<p>
  <strong>Toelichting:</strong><br/>
  <?php print $supplement; ?>
</p>
<?php if ($edit_access || $admin_access) : ?>
  <ul>
  <?php if ($edit_access) : ?>
    <li><?php print l('bewerk amendement', 'ammo/amendment/edit/' . $entity_id, array('query' => drupal_get_destination()))?></li>
  <?php endif; ?>
  <?php if ($admin_access) : ?>
    <li><?php print l('bewerk advies', 'ammo/amendment/advice/' . $entity_id, array('query' => drupal_get_destination()))?></li>
  <?php endif; ?>
  </ul>
<?php endif; ?>
