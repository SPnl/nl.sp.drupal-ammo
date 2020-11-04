<?php global $base_url; ?>
<h3 id="amendment<?php print $entity_id; ?>">Wijzigingsvoorstel nr. <?php print (!empty($chapterized_id)) ? $chapter . '.' . $chapterized_id : $chapter . '.' . $entity_id; ?> (pagina <?php print $page; ?>, regel <?php print $line; ?>)</h3>
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
  <p>Ingediend door <?php print $members_list; ?>.</p>
  <?php $number = count($backers) + count($owners_member); ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Mede ingediend door <?php print $owners_list; ?>.</p>
  <?php endif; ?>
  <p>Ondersteund door 50 leden (voldoende steun).</p>
<?php else: ?>
  <?php if (!empty($owners_branch)) : ?>
    <p>Ingediend door <?php print $owners_list; ?>.</p>
  <?php endif; ?>
<?php endif; ?>
<h4 class="inline-title">Wijzigingsvoorstel tekst:</h4>
<p class="text-limit"><?php print $amendment_text; ?></p>
<?php if (!empty($supplement)) : ?>
<h4 class="inline-title">Toelichting:</h4>
<p class="text-limit"><?php print $supplement; ?></p>
<?php endif; ?>
<?php if (!empty($state) || !empty($advice)) : ?>
  <p>
  <?php if (!empty($state)) : ?>
    <?php if (!$hide_state || $state === 'withdrawn' || $admin_access) : ?>
      <?php $options = ammo_states(); ?>
      <strong>Status:</strong> <?php print strtolower($options[$state]); ?>
      <?php if (!empty($state_supplement)) : ?>
        <br/><?php print $state_supplement; ?>
      <?php endif; ?>
    <?php endif; ?>
  <?php endif; ?>
  <?php if (!empty($advice) && (!$hide_advice || $admin_access)) : ?>
    <?php if (!empty($state)) : ?>
      <br/>
    <?php endif; ?>
    <?php $options = ammo_amendment_advice(); ?>
    <strong>Advies:</strong> <?php print strtolower($options[$advice]); ?>
    <?php if (!empty($advice_supplement)) : ?>
      <br/><?php print $advice_supplement; ?>
    <?php endif; ?>
  <?php endif; ?>
  </p>
<?php endif; ?>

<?php if (empty($no_links)) : ?>
  <?php $dest = (!empty($destination) ? $destination : ammo_get_destination()); ?>
  <ul class="ammo-list">
    <li><a href="#inhoud">^</a></li>
    <?php if (!empty($owners_branch) || !empty($owners_member)) : ?>
      <?php if ($edit_access) : ?>
        <li><?php print l('bewerk', 'ammo/amendment/edit/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
      <?php if (($admin_access && $support_access) || $superadmin_access) : ?>
        <li><?php print l('bewerk advies', 'ammo/amendment/advice/' . $entity_id, array('query' => $dest))?></li>
      <?php endif; ?>
    <?php endif; ?>
    <?php if (!empty($owners_branch) || !empty($owners_member)) : ?>
      <?php if ($withdraw_access) : ?>
        <?php if ($unsupported_branches) : ?>
          <li><?php print l('mede indienen als afdeling', 'ammo/support/add/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php endif; ?>
        <?php if ($supported_branches) : ?>
          <li><?php print l('intrekken als afdeling', 'ammo/support/withdraw/branch/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php endif; ?>
        <?php if ($removable_member_owners) : ?>
          <li><?php print l('intrekken individuele indieners', 'ammo/support/withdraw/branchmembers/amendment/' . $entity_id, array('query' => $dest))?></li>
        <?php endif; ?>
      <?php endif; ?>
    <?php endif; ?>
  </ul>
<?php endif; ?>
