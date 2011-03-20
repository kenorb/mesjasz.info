; $Id: README.txt,v 1.8 2008/10/20 19:57:31 mbutcher Exp $

Overview
--------
The quiz.module is a framework which allows you to create interactive quizzes 
for your visitors. It allows for the creation of questions of varying types, and
to collect those questions into quizzes. 


Requirements
------------
Drupal 6.x
PHP 4.3.0 or greater (5 is strongly recommended)
MySQL 5

The module consists of two types of modules: the Quiz module itself 
(quiz.module), and various question types (example: multichoice.module). The 
main Quiz module and at least one question type module are required to be both
installed and enabled for this module to function properly.


Features
--------
 - Administrative features:
    o Assign feedback to responses to help point out places for further study
    o Supports multiple answers to quiz questions (on supporting question types)
    o Limit the number of takes users are allowed
    o Extensibility allows for additional question types to be added
    o Permissions (create/edit)
    o Randomize questions during the quiz
    o Assign only specific questions from the question bank

 - User features:
   o Can create/edit own quizzes if have 'create quizzes' permission
   o Can take a quiz if have 'view quizzes' permissions, and receive score


Installation
------------
Please refer to the INSTALL file for installation directions.

Support
-------
- Visit the Quiz group at http://groups.drupal.org


Credits
-------
- Specification:      Robert Douglass
- Original author:    K�roly N�gyesi
- Update to Drupal 5: Wim Mostrey and riverfr0zen
- Maintainers: Angela Byron, westwesterson, mbutcher

I'd like to extend a heart-felt thank you to the folks at Google for their 
Summer of Code program, without which my working on this module would not have 
been possible. I'd also like to extend my thanks to Robert Douglass as my mentor 
on this project, for his tireless patience and faith in my abilities, and the 
entire Drupal development community for their support while I struggled with
foreign concepts like 'node' and 'taxonomy.' You guys all provided me with 
support, encouragement, and fun times and I sincerely hope to remain a part of 
the Drupal community for a long time to come!

Also huge thanks go to Ejovi Nuwere of http://www.securitylab.net/ for 
sponsoring further module development after Summer of Code.

-Angela Byron/'webchick'
