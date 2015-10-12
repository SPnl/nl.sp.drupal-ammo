<?php if (!empty($meetings)) : ?>
  <?php if (!empty($meetings[1]['documents'])) : ?>
    <?php foreach ($meetings[1]['documents'] as $document) : ?>
      <h1><?php print $meetings[1]['title']; ?>: <?php print $document['title']; ?></h1>
      <?php foreach ($document['amendments'] as $amendment) : ?>
        <?php if ($amendment->chapter !== $chapter) : ?>
          <h2>Hoofdstuk <?php print $amendment->chapter; ?></h2>
          <?php $chapter = $amendment->chapter; ?>
        <?php endif; ?>
        <?php if ($amendment->page !== $page) : ?>
          <h2>Pagina <?php print $amendment->page; ?></h2>
          <?php $page = $amendment->page; ?>
        <?php endif; ?>
        <?php print theme('amendment', array('entity_id' => $amendment, 'destination' => $destination)); ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  <?php endif; ?>
<?php endif; ?>
