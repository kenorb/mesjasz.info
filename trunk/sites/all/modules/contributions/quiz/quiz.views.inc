<?php
// $Id: quiz.views.inc,v 1.1.2.2 2008/11/19 21:02:37 mbutcher Exp $

/**
 * This file provides Views 2 support.
 * @file
 */
 
/**
 * Implementation of hook_views_data().
 */
function quiz_views_data() {
  return array(
    'quiz_node_results' => array(
      
      // Table Definition
      'table' => array(
        'group' => 'Quiz Results',
        
        // Base tables:
        'base' => array(
          'field' => 'vid',
          'title' => t('Quiz Results'),
          'help' => t('Quiz Results are the results of a quiz a user has taken.'),
          'weight' => 0,
        ),
        'join' => array(
          // This is vid because vid is always more specific.
          'node' => array('left_field' => 'vid', 'field' => 'vid', 'type' => 'INNER'),
        ),
      ),
      
      // Field descriptions:
      'result_id' => array(
        'title' => t('Result ID'),
        'help' => t('The ID of the result.'),
        'field' => array('handler' => 'quiz_views_handler_field_result_id', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'view_handler_argument_numeric',
          'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'result_id',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'nid' => array(
        'title' => t('Quiz Node NID.'),
        'help' => t('The ID of the Quiz Node.'),
        'field' => array('handler' => 'views_handler_field', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'view_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'field' => 'nid',
          'label' => t('node'),
        ),
      ),
      'vid' => array(
        'title' => t('Quiz Node VID.'),
        'help' => t('The Version ID of the Quiz Node.'),
        'field' => array('handler' => 'views_handler_field', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'vid',
        ),
        // Related to NODE
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'field' => 'vid',
          'label' => t('node'),
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'uid' => array(
        'title' => t('User ID.'),
        'help' => t('The ID of the user who took this quiz.'),
        'field' => array('handler' => 'views_handler_field_user', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_user',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        // Related to USERS
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'users',
          'field' => 'uid',
          'label' => t('user'),
        ),
      ),
      'time_start' => array(
        'title' => t('Quiz Start Time.'),
        'help' => t('Time the quiz was started.'),
        'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_date',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'time_end' => array(
        'title' => t('Quiz End Time'),
        'help' => t('Time the quiz was finished.'),
        'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_date',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      //'released' => array(
      //  'title' => t('Released'),
      //  'help' => t('The quiz release status'),
      //),
      'score' => array(
        'title' => t('Score'),
        'help' => t('Score on the Quiz.'),
        'field' => array('handler' => 'views_handler_field', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
    ),
    
    // Quiz Node Properties (additions to Node)
    'quiz_node_properties' => array(
      
      // Table Definition
      'table' => array(
        'group' => 'Quiz',
        
        // Allow attachment to a node:
        'join' => array(
          // This is vid because vid is always more specific.
          'node' => array('left_field' => 'vid', 'field' => 'vid', 'type' => 'INNER'),
        ),
      ),
      
      // Field definitions
      'property_id' => array(
        'title' => t('Quiz node property ID'),
        'help' => 'The unique ID for a property.',
        'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'view_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'field' => 'nid',
          'label' => t('node'),
        ),
      ),
      'nid' => array(
        'title' => t('Quiz Node NID.'),
        'help' => t('The ID of the Quiz Node.'),
        'field' => array('handler' => 'views_handler_field', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'view_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'nid',
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'field' => 'nid',
          'label' => t('node'),
        ),
      ),
      'vid' => array(
        'title' => t('Quiz Node VID.'),
        'help' => t('The Version ID of the Quiz Node.'),
        'field' => array('handler' => 'views_handler_field', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
          'validate_type' => 'vid',
        ),
        // Related to NODE
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'node',
          'field' => 'vid',
          'label' => t('node'),
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'number_of_random_questions' => array(
        'title' => t('Number of random questions.'),
        'help' => t('The number of questions on this quiz that will be randomly selected.'),
        'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'pass_rate' => array(
        'title' => t('Pass rate'),
        'help' => t('The percentage of questions that must be right before the quiz is passed.'),
        'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          //'name_field' => 'title',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'shuffle' => array(
        'title' => t('Shuffle'),
        'help' => t('Indicates whether questions will be shuffled.'),
        'field' => array('handler' => 'views_handler_field_boolean', 'click sortable' => TRUE),
        /*'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
          'validate_type' => 'vid',
        ),*/
      'sort' => array('handler' => 'views_handler_sort'),
      ),
      'backwards_navigation' => array(
        'title' => t('Backwards navigation'),
        'help' => t('Indicates whether quiz takers can go back to previous questions.'),
        'field' => array('handler' => 'views_handler_field_boolean', 'click sortable' => TRUE),
        /*'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),*/
      'sort' => array('handler' => 'views_handler_sort'),
      ),
      'feedback_time' => array(
        'title' => t('Feedback Time'),
        'help' => t('Indicates whether quiz takers will get feedback after every question.'),
        'field' => array('handler' => 'views_handler_field_boolean', 'click sortable' => TRUE),
        /*'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),*/
      'sort' => array('handler' => 'views_handler_sort'),
      ),
      'time_limit' => array(
        'title' => t('Time limit'),
        'help' => t('The time limit on a quiz.'),
        'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'quiz_open' => array(
        'title' => t('Quiz open time'),
        'help' => t('The first time a new quiz can be taken.'),
        'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_date',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'quiz_close' => array(
        'title' => t('Quiz close time'),
        'help' => t('The last time a new quiz can be taken.'),
        'field' => array('handler' => 'views_handler_field_date', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_date',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'quiz_always' => array(
        'title' => t('Always Available'),
        'help' => t('Indicates whether quiz is always available (Open and Close time are ignored).'),
        'field' => array('handler' => 'views_handler_field_boolean', 'click sortable' => TRUE),
        /*'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),*/
      'sort' => array('handler' => 'views_handler_sort'),
      ),
      'takes' => array(
        'title' => t('Number of takes'),
        'help' => t('The number of times a quiz can be taken.'),
        'field' => array('handler' => 'views_handler_field_numeric', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
      ),
      'tid' => array(
        'title' => t('Term ID'),
        'help' => t('The term ID used to select questions based on taxonomy.'),
        'field' => array('handler' => 'views_handler_field_taxonomy', 'click sortable' => TRUE),
        'argument' => array(
          'handler' => 'views_handler_argument_numeric',
          'numeric' => TRUE,
        ),
        'sort' => array('handler' => 'views_handler_sort'),
        
        // Related to TERM
        'relationship' => array(
          'handler' => 'views_handler_relationship',
          'base' => 'term_data',
          'field' => 'tid',
          'label' => t('term'),
        ),
      ), // End tid
    ), // End quiz_node_properties
  ); // End definition
}