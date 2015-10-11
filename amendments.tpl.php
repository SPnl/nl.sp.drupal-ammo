<?php if (!empty($meetings)) : ?>
  <h1><?php print $meetings[1]['title']; ?></h1>
  <?php if (!empty($meetings[1]['documents'])) : ?>
    <?php foreach ($meetings[1]['documents'] as $document) : ?>
      <h2><?php print $document['title']; ?></h2>
      <?php foreach ($document['amendments'] as $amendment) : ?>
        <?php print theme('amendment', array('entity_id' => $amendment['id'])); ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  <?php endif; ?>
<?php endif; ?>
