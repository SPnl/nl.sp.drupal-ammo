<h3 id="motion<?php print $entity_id; ?>">Motie nr. <?php print $entity_id; ?></h3>
<?php if ($first_owner['contact_type'] == 'member') : ?>
  <?php foreach ($owners_member as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $owners_list; ?> van afdeling <?php print $first_owner['branch_display_name']; ?>.</p>
  <?php $number = count($backers); ?>
  <p>Ondersteunt door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?>.</p>
<?php else : ?>
  <?php foreach ($owners_branch as $owner) : $list[] = $owner['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $owners_list; ?>.</p>
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
    <?php if (!$admin_access) : ?>
      <?php if (empty($owners_branch)) : ?>
        <?php if ($backed_by_user) : ?>
          <li><?php print l('trek steun als lid in', 'ammo/support/withdraw/member/motion/' . $entity_id, array('query' => $dest))?></li>
        <?php else  : ?>
          <li><?php print l('steun als lid', 'ammo/support/add/member/motion/' . $entity_id, array('query' => $dest))?></li>
        <?php endif; ?>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($unsupported_branches) : ?>
      <li><?php print l('mede indienen als afdeling', 'ammo/support/add/branch/motion/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if ($supported_branches) : ?>
      <li><?php print l('intrekken als afdeling', 'ammo/support/withdraw/branch/motion/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
  </ul>
<?php endif; ?>
