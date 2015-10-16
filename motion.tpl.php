<h3 id="motion<?php print $entity_id; ?>">Motie nr. <?php print $entity_id; ?></h3>
<?php if (!$backed) : ?>
  <?php foreach ($owners_member as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $owners_list; ?> van <?php print $first_owner['contact_display_name']; ?>.</p>
<?php else : ?>
  <p>Ingediend door <?php print $first_owner['contact_display_name']; ?>.</p>
<?php endif; ?>
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
<?php if ($edit_access || $admin_access || !$backed) : ?>
  <?php if (arg(1) !== 'support') : ?>
    <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
    <ul class="ammo-list">
      <li><a href="#inhoud">^</a></li>
      <?php if ($edit_access) : ?>
        <li><?php print l('bewerk motie', 'ammo/motion/edit/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if ($admin_access) : ?>
        <li><?php print l('bewerk advies', 'ammo/motion/advice/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if (!$backed) : ?>
        <?php if ($backed_by_user) : ?>
          <li><?php print l('trek steun in', 'ammo/support/withdraw/motion/' . $entity_id, array('query' => $dest))?></li>
        <?php else  : ?>
          <?php if (!$admin_access) : ?>
            <li><?php print l('steun', 'ammo/support/add/motion/' . $entity_id, array('query' => $dest))?></li>
          <?php endif; ?>
        <?php endif; ?>
      <?php endif; ?>
    </ul>
  <?php endif; ?>
<?php endif; ?>

