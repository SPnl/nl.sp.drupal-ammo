<html>
<meta http-equiv="Content-Type" content="text/html">
<body>
<?php foreach ($meeting['branches'] as $branch) : ?>
  <?php foreach ($branch['motions'] as $motion) : ?>
    <h1><?php print $meeting['title']; ?> (<?php print $branch['first_owner']['branch_display_name']; ?>)</h1>
    <p><strong>Motie nr. <?php print $motion['id']; ?></strong></p>
    <?php if (!empty($motion['owners_branch'])) : ?>
      <?php $list = array(); ?>
      <?php foreach ($motion['owners_branch'] as $owner_branch) : $list[] = $owner_branch['contact_display_name']; endforeach; ?>
      <?php $last = array_pop($list); ?>
      <?php if (count($list) === 0) : $owners_list = $last; else : $owners_list = implode(', ', $list) . ' en ' . $last; endif; ?>
    <?php endif; ?>
    <?php if (!empty($motion['owners_member'])) : ?>
      <?php $list = array(); ?>
      <?php foreach ($motion['owners_member'] as $owner_member) : $list[] = $owner_member['contact_display_name']; endforeach; ?>
      <?php $last = array_pop($list); ?>
      <?php if (count($list) === 0) : $members_list = $last; else : $members_list = implode(', ', $list) . ' en ' . $last; endif; ?>
      <p>Ingediend door <?php print $members_list; ?> van afdeling <?php print $owner_member['branch_display_name']; ?>.</p>
      <?php $number = count($motion['backers']) + count($motion['owners_member']); ?>
      <?php if (!empty($motion['owners_branch'])) : ?>
        <p>Mede ingediend door afdeling <?php print $owners_list; ?>.</p>
      <?php endif; ?>
    <p>Ondersteund door <?php print $number; ?> <?php print ($number == 1) ? 'lid' : 'leden'; ?><?php if (empty($motion['owners_branch'])) : print ' ('; print ($number >= 5) ? 'voldoende' : 'onvoldoende'; print ' steun)'; endif ?>.</p>
    <?php else: ?>
      <?php if (!empty($motion['owners_branch'])) : ?>
        <p>Ingediend door afdeling <?php print $owners_list; ?>.</p>
      <?php endif; ?>
    <?php endif; ?>
    <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
    <p><strong><?php print $motion['consideration_opening']; ?></strong> <?php print $motion['consideration_body']; ?></p>
    <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
    <p><strong><?php print $motion['follow_up_opening']; ?></strong> <?php print $motion['follow_up_body']; ?></p>
    <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
    <?php if (!empty($motion['supplement'])) : ?>
      <p><strong>Toelichting</strong></p>
      <p><?php print $motion['supplement']; ?></p>
      <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
    <?php endif; ?>
    <?php $options = ammo_states(); ?>
    <p><strong>Status:</strong> <?php print $options[$motion['state']]; ?></p>
    <?php if (!empty($motion['advice'])) : ?>
      <?php $options = ammo_motion_advice(); ?>
      <p><strong>Advies:</strong> <?php print $options[$motion['advice']]; ?></p>
      <?php if (!empty($motion['advice_supplement'])) : ?>
        <p><?php print $motion['advice_supplement']; ?></p>
      <?php endif; ?>
      <?php if ($variables['values']['notespace']) print $meeting['notespacetext'];?>
    <?php endif; ?>
    <br style="page-break-before: always">
  <?php endforeach; ?>
<?php endforeach; ?>
</body>
</html>

