<h3 id="amendment<?php print $entity_id; ?>">Amendement nr. <?php print $entity_id; ?></h3>

<div class="ammo-hidden">
<?php if (!$backed) : ?>
  <?php foreach ($owners_member as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $owners_list; ?> van <?php print $first_owner['contact_display_name']; ?>.</p>
<?php else : ?>
  <p>Ingediend door <?php print $first_owner['contact_display_name']; ?>.</p>
<?php endif; ?>
<?php switch ($type): ?>
<?php case 'specific': ?>
<p>Regel <?php print $line; ?></p>
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
<?php if (!empty($advice)) : ?>
<?php $options = ammo_amendment_advice(); ?>
  <p>
    <strong>Advies:</strong> <?php print strtolower($options[$advice]); ?>
    <?php if (!empty($advice_supplement)) : ?>
      <br/><?php print $advice_supplement; ?>
    <?php endif; ?>
  </p>
<?php endif; ?>
</div>

<?php if ($backed) : ?>
<?php endif; ?>
<?php if ($edit_access || $admin_access || !$backed) : ?>
  <?php if (arg(1) !== 'support') : ?>
    <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
    <ul class="ammo-list">
      <?php //<li><a href="#" class="ammo-toggle">bekijk</a></li> ?>
      <li><a href="#inhoud">^</a></li>
      <?php if ($edit_access) : ?>
        <li><?php print l('bewerk', 'ammo/amendment/edit/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if ($admin_access) : ?>
        <li><?php print l('bewerk advies', 'ammo/amendment/advice/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if (!$backed) : ?>
        <?php if ($backed_by_user) : ?>
          <li><?php print l('trek steun in', 'ammo/support/withdraw/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php else  : ?>
          <?php if (!$admin_access) : ?>
            <li><?php print l('steun', 'ammo/support/add/amendment/' . $entity_id, array('query' => $dest))?></li>
          <?php endif; ?>
        <?php endif; ?>
      <?php endif; ?>
    </ul>
  <?php endif; ?>
<?php endif; ?>
