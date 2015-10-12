<?php if (!empty($meetings)) : ?>
  <h1><?php print $meetings[1]['title']; ?></h1>
  <?php if (!empty($meetings[1]['motions'])) : ?>
    <?php foreach ($meetings[1]['motions'] as $motion) : ?>
        <?php print theme('motion', array('entity_id' => $motion, 'destination' => $destination)); ?>
    <?php endforeach; ?>
  <?php endif; ?>
<?php endif; ?>
