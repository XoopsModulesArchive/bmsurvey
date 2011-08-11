<?php
// $Id: admin.php,v 1.2 2007/07/24 10:17:04 yoshis Exp $
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2005 Yoshi.Sakai @ Bluemoon inc.         //
//                       <http://www.bluemooninc.biz/>                       //
// ------------------------------------------------------------------------- //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
define("_AM_BMSURVEY_ERROR01", "Error: not writable");
// Admin Top Menu
define("_AM_PREFERENCES","Preferences");
define("_AM_BMSURVEY_GOMOD","Public Page");
define("_AM_BMSURVEY_FAQ","FAQs");
define("_AM_BMSURVEY_SUPPORTSITE","Author");
// Admin Tag menu
define('_AM_BMSURVEY_MANAGE','Surveys');
define("_AM_BMSURVEY_RESPONDENT","Email Responses");
define("_AM_BMSURVEY_CASTSURVEY","Send Question");
define("_AM_BMSURVEY_CHECKRESPONSE","Get Emails");
define("_AM_BMSURVEY_RESISTER","Request Registration");
define("_AM_BMSURVEY_STATUS","System Info");
// Document Link
define("_AM_BMSURVEY_DOC_POPNUPBLOG","Info: PopnupBlog Integration");
define("_AM_BMSURVEY_DOC_UPDATEINFO","Info: Upgrade Instructions");
define('_AM_BMSURVEY_DOC_MAILTO','Info: Mail Options');
// Admin Body
define("_AM_BMSURVEY_RESPONDENTS","Email Responses");
define("_AM_BMSURVEY_RESPONDENT_USAGE","Do not edit fields with astericks (*). This is a system field.<br />Ticket Number changes each time questions are sent.");
define("_AM_BMSURVEY_USERNAME","Login Name");
define("_AM_BMSURVEY_PASSWORD","Ticket Number");
define("_AM_BMSURVEY_FNAME","First Name");
define("_AM_BMSURVEY_LNAME","Last Name");
define("_AM_BMSURVEY_EMAIL","Email Address");
define("_AM_BMSURVEY_DISABLED","Disabled");
define("_AM_BMSURVEY_SURVEYID","Survey ID");
define("_AM_BMSURVEY_RESPONSEID","Response ID");
define("_AM_BMSURVEY_CHANGED","Date Modified");
define("_AM_BMSURVEY_EXPIRE","Date Expires");
define("_AM_BMSURVEY_INVITATION","Send email to respondents requesting registration information.");
define("_AM_BMSURVEY_SUBJECT","Subject");
define("_AM_BMSURVEY_SUBJECT_NEW","Survey Registration");
define("_AM_BMSURVEY_MESSAGE","Body");
define("_AM_BMSURVEY_MESSAGE_NEW","\nThis registration is required to participate in a future survey. \nPlease reply to this message and place your answers below inside the brackets [].\n\n~~~~\n\nu:[] Login Name\nf:[] First Name\nl:[] Last Name\ns:[] Survey ID\nd:[] Expiration Date for response");
define("_AM_BMSURVEY_CHOSESURVEY","Choose Survey");
define("_AM_BMSURVEY_SENDQUESTION","Send a Question");
define("_AM_BMSURVEY_CONFIRM","Confirm Respondents");
define("_AM_BMSURVEY_SENDQUESTIONNOW","Send Questions");
define("_AM_BMSURVEY_SENDQUESTIONUSAGE","Note: if new questions are sent, ticket numbers will be updated, and old questions becomes unrecoverable.<br /><br />\n\nAutomate using cron/wget with this URL:<br />\n%s<br /><br />\n\nAdd '&hide=1' to omit the question sentence");
define("_AM_BMSURVEY_CHECKRESPONSENOW","Check Mail Responses");
define("_AM_BMSURVEY_CHECKRESPONSEUSAGE","Automate using cron/wget with this URL:<br />\n%s");

define("_AM_BMSURVEY_SEEARESULT","View Response");
define('_AM_BMSURVEY_COPYQUESTION','Copy From');
define('_AM_BMSURVEY_SELECTSTATUS','-Select-');
define('_AM_BMSURVEY_RATECOUNT','Count Each Rate');
define('_AM_BMSURVEY_NORESPONSE','No Response');
define('_AM_BMSURVEY_TOTAL','Total');
define('_AM_BMSURVEY_QUESTIONNUMBER','Q No');
define('_AM_BMSURVEY_FILEDNAME_DESC','Filename Description');
define('_AM_BMSURVEY_ARCHIVED','Archived');
define('_AM_BMSURVEY_TEST','Test');
define('_AM_BMSURVEY_EXPIRATION','Expires');
define('_AM_BMSURVEY_ACTIVE','Active');
define('_AM_BMSURVEY_EDIT','Edit');

?>
