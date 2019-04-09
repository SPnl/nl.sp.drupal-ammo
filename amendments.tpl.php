<h2 id="inhoud">Overzicht alle ingediende wijzigingsvoorstelen</h2>

<?php if (!empty($meeting['documents']) && empty($meeting['hide'])) : ?>
  <ul class="first navigation">
    <?php foreach ($meeting['documents'] as $document) : ?>

      <li class="document">

        <a href="#document<?php print $document['id']; ?>"><strong>Document: <?php print $document['title']; ?></strong></a>
        <?php if (!empty($document['chapters'])): ?>
          <ul class="chapters tab">
            <li class="chapter"><span>Hoofdstuk:</span></li>
            <?php foreach ($document['chapters'] as $chapter) : ?>
              <?php if (!isset($firstchapter)) $firstchapter = $chapter['nr']; ?>
              <li class="chapter">
                <a href="#" class="tablinks <?php print ($firstchapter == $chapter['nr']) ? 'active' : ''; ?>" id="<?php print 'tab-'.$document['id'].'-'.$chapter['nr']; ?>"><?php print $chapter['nr']; ?></a>
              </li>
            <?php endforeach; ?>
          </ul>
          <?php foreach ($document['chapters'] as $chapter) : ?>
          <div id="<?php print 'tabcontent-'.$document['id'].'-'.$chapter['nr']; ?>" class="tabcontent <?php print ($firstchapter == $chapter['nr']) ? 'active' : ''; ?>">
              <ul class="amendments">
                <li class="amendment">
                  <a href="#chapter<?php print $chapter['nr']; ?>">Hoofdstuk <?php print $chapter['nr']; ?></a>
                </li>
              </ul>
              <ul class="amendments">
                <?php $amendment_ids = $document['amendment_index'][$chapter['nr']]; ?>
                <?php foreach ($amendment_ids as $amendment_id) : ?>
                  <?php $amendment = $document['amendments'][$amendment_id]; ?>
                  <li class="amendment">
                    <a href="#amendment<?php print $amendment['id']; ?>"><?php print (!empty($amendment['chapterized_id'])) ? $amendment['chapter'] . '.' . $amendment['chapterized_id'] : $amendment['chapter'] . '.' . $amendment_id; ?></a>
                  </li>
                <?php endforeach; ?>
              </ul>
            </div>
          <?php endforeach; ?>
        <?php endif; ?>
      </li>
    <?php endforeach; ?>
  </ul>

  <ul class="first">
    <?php foreach ($meeting['documents'] as $document) : ?>
    <li>
        <h2 id="document<?php print $document['id']; ?>" class="document-title"><?php print $document['title']; ?></h2>
        <ul>
          <?php if ($meeting['admin_access']) : ?>
            <?php $states = ammo_states(); ?>
            <?php foreach ($meeting['documents'][$document['id']]['totals'] as $key => $value) : ?>
              <?php $rows[] = array($states[$key], $value); ?>
            <?php endforeach; ?>
            <?php print theme('table', array('rows' => $rows)); ?>
          <?php endif; ?>

          <?php if (!empty($document['chapters'])): ?>
            <?php foreach ($document['chapters'] as $chapter) : ?>
              <li>
                <h3 id="chapter<?php print $chapter['nr']; ?>" class="chapter-nr">Hoofdstuk <?php print $chapter['nr']; ?></h3>
                <ul>
                  <?php foreach ($chapter['pages'] as $page) : ?>
                    <li>
                      <h3 id="page<?php print $page['nr']; ?>" class="page-nr">Pagina <?php print $page['nr']; ?></h2>
                      <ul>
                        <?php foreach ($page['amendments'] as $amendment_id) : ?>
                          <?php $amendment = $document['amendments'][$amendment_id]; ?>
                          <li class="ammo-element <?php print $amendment['state']; ?>">
                            <?php print theme('amendment', array('entity_id' => $amendment['id'], 'destination' => $destination)); ?>
                          </li>
                        <?php endforeach; ?>
                      </ul>
                    </li>
                  <?php endforeach; ?>
                </ul>
              </li>
            <?php endforeach; ?>
          <?php endif; ?>
        </ul>
      </li>
    <?php endforeach; ?>
  </ul>

<?php endif; ?>
<hr/>
