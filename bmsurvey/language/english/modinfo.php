<?php
// $Id: modinfo.php,v 1.1.1.1 2005/08/10 12:14:04 yoshis Exp $
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
define("_MI_SURVEY_NAME","BmSurvey");
define("_MI_SURVEY_ADMIN","Management");
// A brief description of this module
define("_MI_SURVEY_BNAME1","Survey Form");
define("_MI_SURVEY_DESC","Bluemoon.Multi-Survey");

// Names of blocks for this module
define('_MI_MSURVEY_RESPONDENT','Edit Respondents');
define('_MI_MSURVEY_CASTSURVEY','Send Question');
define('_MI_MSURVEY_RECIEVECHECK','Recieve Response');
define('_MI_MSURVEY_RESISTER','Send Resister mail');
define('_MI_MSURVEY_STATUS','Status Check');
define('_MI_MSURVEY_FILECHARSET', 'Character-code for attach file');
define('_MI_MSURVEY_FILECHARSET_DESC', 'Set the character-code for save to server. (ASCII,UTF-8,EUC etc)');
define('_MI_MSURVEY_CSVCHARSET', 'Character-code for CSV file');
define('_MI_MSURVEY_CSVCHARSET_DESC', 'Set the character-code for CSV download to cliants. (ASCII,UTF-8,EUC etc)');
define('_MI_MSURVEY_CSVADDNUM', 'Add number for CSV');
define('_MI_MSURVEY_CSVADDNUM_DESC', 'Add number for CSV oputput header.(line 1)');
define('_MI_MSURVEY_CHOICEOPT','Type of choice output on CSV');
define('_MI_MSURVEY_CHOICEOPT_DESC','Select type as strings or sequential number.');
define('_MI_MSURVEY_CSVOTHERF','!other format on CSV output');
define('_MI_MSURVEY_CSVOTHERF_DESC','Set CSV output format for !other responce.');
define('_MI_MSURVEY_MAILSERVER', 'POP3 mail server');
define('_MI_MSURVEY_MAILSERVER_DESC', 'Set the POP3 server for recieve mail.');
define('_MI_MSURVEY_MAILUSER', 'POP3 User name');
define('_MI_MSURVEY_MAILUSER_DESC', 'Set user name for POP3.');
define('_MI_MSURVEY_MAILPWD', 'POP3 Password');
define('_MI_MSURVEY_MAILPWD_DESC', 'Set pasword for POP3.');
define('_MI_MSURVEY_MAILADDR', 'Mail Address');
define('_MI_MSURVEY_MAILADDR_DESC', 'Set mail address for POP3 and SMTP From');
define('_MI_MSURVEY_CASTKEY', 'Cast Key');
define('_MI_MSURVEY_CASTKEY_DESC', 'Set key strings for cast.php.');
define('_MI_MSURVEY_MANAGEGROUP', 'Management Groups');
define('_MI_MSURVEY_MANAGEGROUP_DESC', 'Set groups for survey management');
define('_MI_MSURVEY_MGPSTATUS', 'Manage group status');
define('_MI_MSURVEY_MGPSTATUS_DESC', 'For group permission to edit,activate and end.');
define('_MI_MSURVEY_BLOCKLIST', 'Block list number');
define('_MI_MSURVEY_BLOCKLIST_DESC', 'Number of list at block');
define('_MI_MSURVEY_ADDINFO', 'Add info to response');
define('_MI_MSURVEY_ADDINFO_DESC', 'Send response mail with additional info');
define('_MI_MSURVEY_ADDUSAGE', 'Add usage to questionnaire');
define('_MI_MSURVEY_ADDUSAGE_DESC', 'Send questionnaire mail with usage info');
define('_MI_MSURVEY_ONERESPONSE', 'Accept One Response');
define('_MI_MSURVEY_ONERESPONSE_DESC', 'Accept one response for one question. No = Accept all responses');
define('_MI_MSURVEY_RESETRADIOBUTTON', 'Reset for radio button');
define('_MI_MSURVEY_RESETRADIOBUTTON_DESC', 'It can be reset for radio button');
define('_MI_MSURVEY_RESULTRANK', 'Rate report type at view a survey report');
define('_MI_MSURVEY_RESULTRANK_DESC', 'Select avarage or count');

?>