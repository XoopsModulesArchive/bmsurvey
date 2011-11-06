<?php
// $Id$
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
// The name of this module
define("_MI_SURVEY_NAME","Survey");
define("_MI_SURVEY_ADMIN","Management");
// A brief description of this module
define("_MI_SURVEY_BNAME1","Survey Form");
define("_MI_SURVEY_DESC","Survey (phpESP) - by Bluemoon");

// Names of blocks for this module
define('_MI_MSURVEY_RESPONDENT','Edit Respondents');
define('_MI_MSURVEY_CASTSURVEY','Send Question');
define('_MI_MSURVEY_RECIEVECHECK','Receive Response');
define('_MI_MSURVEY_RESISTER','Request Registration');
define('_MI_MSURVEY_STATUS','System Info');
define('_MI_MSURVEY_FILECHARSET', 'Character Encoding - Attached Files');
define('_MI_MSURVEY_FILECHARSET_DESC', 'Set the character encoding for saving attached files to the server (ASCII,UTF-8,EUC etc)');
define('_MI_MSURVEY_CSVCHARSET', 'Character Encoding - Downloads');
define('_MI_MSURVEY_CSVCHARSET_DESC', 'Set the character encoding for downloading of exported survey data (ASCII,UTF-8,EUC etc)');
define('_MI_MSURVEY_CSVADDNUM', 'Add Number for CSV');
define('_MI_MSURVEY_CSVADDNUM_DESC', 'Add Number for Exported Data Header (line 1)');
define('_MI_MSURVEY_CHOICEOPT','Exported Output Type');
define('_MI_MSURVEY_CHOICEOPT_DESC','Select type as strings or sequential number');
define('_MI_MSURVEY_CSVOTHERF','!other Field Export Format');
define('_MI_MSURVEY_CSVOTHERF_DESC','Set output format for !other field responses');
define('_MI_MSURVEY_MAILSERVER', 'Email - Server');
define('_MI_MSURVEY_MAILSERVER_DESC', 'Specify the POP3 server that will recieve emailed responses');
define('_MI_MSURVEY_MAILUSER', 'Email - Login Name');
define('_MI_MSURVEY_MAILUSER_DESC', 'Set email server user name');
define('_MI_MSURVEY_MAILPWD', 'Email - Password');
define('_MI_MSURVEY_MAILPWD_DESC', 'Set email server pasword');
define('_MI_MSURVEY_MAILADDR', 'Email - Address');
define('_MI_MSURVEY_MAILADDR_DESC', 'Set FROM email address');
define('_MI_MSURVEY_CASTKEY', 'Cast Key');
define('_MI_MSURVEY_CASTKEY_DESC', 'Set key strings for cast.php');
define('_MI_MSURVEY_MANAGEGROUP', 'Management Groups');
define('_MI_MSURVEY_MANAGEGROUP_DESC', 'Groups allowed to manage surveys');
define('_MI_MSURVEY_MGPSTATUS', 'Manage Survey Status');
define('_MI_MSURVEY_MGPSTATUS_DESC', 'Allow survey managers to edit, activate, or end surveys?');
define('_MI_MSURVEY_BLOCKLIST', 'Block Listing Quantity');
define('_MI_MSURVEY_BLOCKLIST_DESC', 'Number of surveys to list in block');
define('_MI_MSURVEY_ADDINFO', 'Additional Response Info');
define('_MI_MSURVEY_ADDINFO_DESC', 'Include additional info in response notifcation emails?');
define('_MI_MSURVEY_ADDUSAGE', 'Send Usage Info in Email Questionnaire');
define('_MI_MSURVEY_ADDUSAGE_DESC', 'Include survey usage info in emailed questionnaires?');
define('_MI_MSURVEY_ONERESPONSE', 'Allow Single Response');
define('_MI_MSURVEY_ONERESPONSE_DESC', 'Accept only one response for each question (No = accept all responses)?');
define('_MI_MSURVEY_RESETRADIOBUTTON', 'Reset - Radio Buttons');
define('_MI_MSURVEY_RESETRADIOBUTTON_DESC', 'Show Reset for Radio Button questions allowing choices to be reset as initially displayed');
define('_MI_MSURVEY_RESULTRANK', 'Reporting - Rate Results Type');
define('_MI_MSURVEY_RESULTRANK_DESC', 'How rate field types are displayed when viewing reports');

?>
