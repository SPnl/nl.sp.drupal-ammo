<h3 id="motion<?php print $entity_id; ?>">Motie nr. <?php print $entity_id; ?></h3>
<?php //print '<p>' . $meeting_title . ' van de SP, in vergadering bijeen op ' . $meeting_date . '.</p>'; ?>
<?php if (!empty($owners_branch)) : ?>
  <?php $list = array(); ?>
  <?php foreach ($owners_branch as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
<?php endif; ?>
<?php if (!empty($owners_member)) : ?>
  <?php $list = array(); ?>
  <?php foreach ($owners_member as $owner_member) : $list[] = $owner_member['contact_display_name']; endforeach; ?>
  <?php $last = array_pop($list); ?>
  <?php if (count($list) === 0) : $members_list = $last; else : $members_list = implode(', ', $list) . ' en ' . $last; endif; ?>
  <p>Ingediend door <?php print $members_list; ?> van afdeling <?php print $owner_member['branch_display_name']; ?>.</p>
  <?php $number = count($backers) + count($owners_member); ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Mede ingediend door afdeling <?php print $owners_list; ?>.</p>
  <?php endif; ?>
  <p>Ondersteunt door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?>.</p>
<?php else: ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Ingediend door afdeling <?php print $owners_list; ?>.</p>
  <?php endif; ?>
<?php endif; ?>
<p>
  <strong><?php print $consideration_opening; ?></strong><br/>
  <?php print $consideration_body; ?><br/>
  <strong><?php print $follow_up_opening; ?></strong><br/>
  <?php print $follow_up_body; ?>
</p>
<?php if (!empty($supplement)) : ?>
  <p>
    <strong>Toelichting:</strong><br/>
    <?php print $supplement; ?>
  </p>
<?php endif; ?>
<?php if (!empty($advice)) : ?>
<?php $options = ammo_motion_advice(); ?>
  <p>
    <strong>Advies:</strong> <?php print strtolower($options[$advice]); ?>
    <?php if (!empty($advice_supplement)) : ?>
      <br/><?php print $advice_supplement; ?>
    <?php endif; ?>
  </p>
<?php endif; ?>
<?php if ($no_links !== TRUE) : ?>
  <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
  <ul class="ammo-list">
    <li><a href="#inhoud">^</a></li>
    <?php if ($edit_access) : ?>
      <li><?php print l('bewerk motie', 'ammo/motion/edit/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if (($admin_access && $support_access) || $superadmin_access) : ?>
      <li><?php print l('bewerk advies', 'ammo/motion/advice/' . $entity_id, array('query' => $dest))?></li>
    <?php endif; ?>
    <?php if (!$admin_access) : ?>
      <?php if ($support_access) : ?>
        <?php if (!empty($owners_member)) : ?>
          <?php if ($backed_by_user) : ?>
            <li><?php print l('trek steun als lid in', 'ammo/support/withdraw/member/motion/' . $entity_id, array('query' => $dest))?></li>
          <?php else  : ?>
            <li><?php print l('steun als lid', 'ammo/support/add/member/motion/' . $entity_id, array('query' => $dest))?></li>
          <?php endif; ?>
        <?php endif; ?>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($withdraw_access) : ?>
      <?php if ($unsupported_branches) : ?>
        <li><?php print l('mede indienen als afdeling', 'ammo/support/add/branch/motion/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if ($supported_branches) : ?>
        <li><?php print l('intrekken als afdeling', 'ammo/support/withdraw/branch/motion/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
    <?php endif; ?>
  </ul>
<?php endif; ?>
