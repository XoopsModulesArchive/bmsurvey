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
define('_MD_BMSURVEY_LIST_CHECKED', 'Checked');
define('_MD_BMSURVEY_LIST_TITLE', 'Title');
define('_GT_From_Option','From address');
define('_MD_FROM_OPTION','From address (Survey address is set at preferences)');
define('_MD_FROM_OPTION_0','Survey address');
define('_MD_FROM_OPTION_1','Users address');
define('_MD_FROM_OPTION_2',"In the questionnaire");
define('_GT_Default_Response','Default Responce');
define('_MD_FROM_DEFRES',"Default Responce ID (Fill Input Example)");
define('_MD_BMSURVEY_LIST_UNAME', 'Owner');
define('_MD_BMSURVEY_LIST_DATE', 'Date');
define('_MD_BMSURVEY_LIST_SUBTITLE', 'Sub Title');
define('_MD_BMSURVEY_THANKS_ENTRY', 'Thank you for completing the survey!');
define('_MD_BMSURVEY_CAN_WRITE_USER_ONLY', 'Guest cannot edit form!');
define('_MD_ASTERISK_REQUIRED', 'Questions marked with a <span class="required">*</span> are required.');
define('_MD_MAIL_TITLE', 'Survey Response');
define("_MD_DENYRESULT","Do not include this result");
define("_MD_DENYRESULTSURE","Deny this result. Are you sure?");
define("_MD_DENYRESULTDONE","Result not included");
define('_MD_DEFAULTRESULT','Set default input for this result');
define('_MD_DEFAULTRESULTDONE','Set to default');
define('_MD_RESPONDENT','Respondent');
define('_MD_QUESTION_OTHER','Other');
define('_MD_BMSURVEY_FORMATERR', ' is not correctly input.');
define('_MD_BMSURVEY_DIGITERR', ' is not a digit.');
define('_MD_BMSURVEY_MAXOVER', ' may not accept over %u.');
define('_MD_BMSURVEY_CHECKANY', '(Check any)');
define('_MD_BMSURVEY_CHECKLIMIT', '(Max %u)');
define('_MD_BMSURVEY_CHECKRESET', 'Reset');

define('_MD_SUBMIT_SURVEY', 'Submit');
define('_MD_NEXT_PAGE', 'Continue');

define('_MD_POP_KEY_M','Member');
define('_MD_POP_KEY_U','Usage');
define('_MD_POP_KEY_Q','Questionnaire');
define('_MD_POP_KEY_ERR','POP-Key Error');
define('_MD_POP_CMD_NEW','Register');
define('_MD_POP_CMD_INP','Response');
define('_MD_POP_CMD_DEL','Delete');
define('_MD_POP_MNEW_ENTRY','Registerd user name as %s.');
define('_MD_POP_MNEW_AREADY','The user name is already in use; please choose another name.');
define('_MD_POP_QINP_HEADER','Please make the reply mail, input between parentheses, and transmit.
Two or more parentheses that exist in one line are choice items. () are single, [] are multiple. 
Only one [] a line are the text input items. Please input the character string. 
----

');
define('_MD_POP_QINP_FAILEDLOGIN','Erro: either the login name or ticket number is incorrect.');
define('_MD_POP_QINP_SUCCEEDED','%s, Your answer was registered. ');
define('_MD_POP_QINP_DELETEIT','This questionnaire has already been answered.
It is possible to delete it by replying to this mail.');
define('_MD_POP_QDEL_SUCCEEDED','%s, Your answer was deleted.');

define("_AM_BMSURVEY_SEEARESULT","See a result");
define('_AM_BMSURVEY_COPYQUESTION','Copy question from survey');
define('_AM_BMSURVEY_SELECTSTATUS','Select Status');
define('_AM_BMSURVEY_RATECOUNT','Count as each rate');
define('_AM_BMSURVEY_NORESPONSE','No Response');
define('_AM_BMSURVEY_TOTAL','Total');
define('_AM_BMSURVEY_QUESTIONNUMBER','Q No');
define('_AM_BMSURVEY_FILEDNAME_DESC','');
define('_AM_BMSURVEY_ARCHIVED','Archived');
define('_AM_BMSURVEY_TEST','Test');
define('_AM_BMSURVEY_EXPIRATION','Expiration');
define('_AM_BMSURVEY_ACTIVE','Active');
define('_AM_BMSURVEY_EDIT','Edit');
//
// From /locale/messages.po
//
define("_GT_Unable_to_open_include_file","ERROR: Unable to open include file. Check INI settings. Aborting.");
define("_GT_Service_Unavailable","Service Unavailable");
define("_GT_Your_progress_has_been_saved","Your progress has been saved. You may return at any time - simply bookmark the link below.<br />If required, you will be prompted to login.");
define("_GT_Resume_survey","Resume Survey");
define("_GT_Invalid_argument","Invalid argument");
define("_GT_Error_opening_survey","Error opening survey.");
define("_GT_Error_opening_surveys","Error opening surveys.");
define("_GT_No_responses_found","No responses found.");
define("_GT_TOTAL","TOTAL");
define("_GT_No_questions_found","No questions found.");
define("_GT_Page_d_of_d","Page %d of %d"); // %d - page, $d - total pages
define("_GT_Yes","Yes");
define("_GT_No","No");
define("_GT_1","1");
define("_GT_2","2");
define("_GT_3","3");
define("_GT_4","4");
define("_GT_6","6");
define("_GT_N/A","N/A");
define("_GT_Page","Page");
define("_GT_of","of");
define("_GT_Error_system_table_corrupt","ERROR: system table corrupt");
define("_GT_Table","Table");
define("_GT_Report_for","Report for");
define("_GT_ID","ID");
define("_GT_Num","#");
define("_GT_Req_d","Required");
define("_GT_Public","Public");
define("_GT_Content","Content");
define("_GT_Previous","Previous");
define("_GT_Next","Next");
define("_GT_Navigate_Individual_Respondent_Submissions","Navigate Individual Respondent Submissions");
define("_GT_Error_cross_analyzing_Question_not_valid_type","ERROR: Question not valid rror cross-analyzing");
define("_GT_Cross_analysis_on_QID","Cross analysis on QID");
define("_GT_Sorry_please_fill_out_the_name","ERROR: enter name, group, and title");
define("_GT_Sorry_name_already_in_use","ERROR: name already in use");
define("_GT_Sorry_that_name_is_already_in_use","Sorry, that name is already in use.");
define("_GT_Warning_error_encountered","Error(s) encountered.");
define("_GT_Please_enter_text","Enter text for this question.");
define("_GT_Sorry_you_must_select_a_type_for_this_question","ERROR: you must select a type");
define("_GT_New_Field","New Field");
define("_GT_Sorry_you_cannot_change_between_those_types_of_question","ERROR: Unable to change between those question types; create a new question instead.");
define("_GT_Sorry_you_need_at_least_one_answer_option_for_this_question_type","ERROR: you must specify at least one answer option for this question type.");
define("_GT_Error_cross_tabulating","Error cross-tabulating.");
define("_GT_Error_same_question","Ensure that column and row selections are not from the same question.");
define("_GT_Error_column_and_row","Ensure both a column and a row are selected.");
define("_GT_Error_analyse_and_tabulate","It is not possible to both cross analyze and tabulate at the same time.");
define("_GT_Error_processing_survey_Security_violation","Error processing survey: Security violation.");
define("_GT_Unable_to_execute_access","Unable to execute query for access.");
define("_GT_Unable_to_execute_respondents","Unable to execute query respondents.");
define("_GT_Unauthorized","Unauthorized");
define("_GT_Incorrect_User_ID_or_Password","Incorrect User ID or Password, or your account has been disabled/expired.");
define("_GT_Your_account_has_been_disabled","Your account has been disabled or you have already completed this survey.");
define("_GT_Unable_to_load_ACL","Unable to load ACL.");
define("_GT_Management_Interface","Management Interface");
define("_GT_This_account_does_not_have_permission","This account does not have permission");
define("_GT_Go_back_to_Management_Interface","Survey Management");
define("_GT_Submit","Submit");
define("_GT_Rank","Rank");
define("_GT_Response","Response");
define("_GT_Average_rank","Average rank");
define("_GT_You_are_missing_the_following_required_questions","You must answer the following required questions");
define("_GT_Survey_Design_Completed","Survey Design Completed");
define("_GT_You_have_completed_this_survey_design","You have completed this survey design.");
define("_GT_To_insert_this_survey_into_your_web_page","To insert this survey into your web page, copy the text below, and paste it into the HTML of your page.");
define("_GT_Once_activated_you_can_also_access_the_survey_directly_from_the_following_URL","Once activated you can also access the survey directly from the following URL.");
define("_GT_You_must_activate_this_survey","You must activate this survey before you can collect results. Once a survey is active, you may no longer make any changes to it. You may activate this survey by choosing <b>Change the Status of an Existing Survey</b> from the Management Interface.");
define("_GT_The_information_on_this_tab_applies_to_the_whole_survey","The information on this tab applies to the whole survey. Fill out this page then go to the <b>Fields</b> tab to edit individual fields.");
define("_GT_Name","SurveyName");
define("_GT_Required","Required");
define("_GT_Survey_filename","Survey Name:");
define("_GT_This_is_used_for_all_further_access_to_this_survey","this codename is used by the system for all access to this survey");
define("_GT_no_spaces","no spaces");
define("_GT_alpha_numeric_only","alpha-numeric only");
define("_GT_Owner","Owner");
define("_GT_User_and_Group_that_owns_this_survey","User and Group that owns this survey");
define("_GT_Title","Title");
define("_GT_Title_of_this_survey","Displayed Title:");
define("_GT_This_appears_at","appears at the top of every page");
define("_GT_free_form_including_spaces","spaces ok");
define("_GT_Subtitle","Sub-Title");
define("_GT_Subtitle_of_this_survey","Displayed Sub-Title:");
define("_GT_Appears_below_the_title","appears below the title on every page");
define("_GT_Additional_Info","Text");
define("_GT_Text_to_be_displayed_on_this_survey_before_any_fields","Text displayed on every page (HTML OK)");
define("_GT_Confirmation_Page","Confirmation Page");
define("_GT_URL","(URL)");
define("_GT_The_URL_to_which_a_user_is_redirected_after_completing_this_survey","Page to which user is redirected after completing this survey");
define("_GT_OR","OR");
define("_GT_heading_text","Heading Text");
define("_GT_body_text","Body Text (HTML OK)");
define("_GT_Heading_in_bold","Heading and Body texts for a Confirmation Page displayed after users completes the survey");
define("_GT_URL_if_present","(URL, if present, takes precedent over texts)");
define("_GT_Email","Email");
define("_GT_Sends_a_copy","Sends each submission to this email address (single address, only, blank for no emails)");
define("_GT_Theme","Theme");
define("_GT_Select_a_theme","Select a display style for this survey");
define("_GT_Options","Options");
define("_GT_Allow_to_save","Allow users to save/resume surveys (requires logins)");
define("_GT_Allow_to_forward","Allow users to navigate between survey sections");
define("_GT_Change_the_order","Change the order of questions by changing the drop-down position to the left of each question. Add Page Breaks below.");
define("_GT_Section_Break","----- Page Break -----");
define("_GT_Remove","Remove");
define("_GT_Edit","Edit");
define("_GT_Add_Section_Break","Add Page Break");
define("_GT_This_is_a_preview","This is a preview of how this survey will display to users. In this preview mode survey navigation buttons are inactive; use the page number buttons to navigate between pages. Some navigation buttons may not appear on the live survey (depends on permissions and options specified). The live survey background color will depend upon the theme and may be different than what you see here. If the survey displays as desired, click the <b>Finish</b> button at the bottom of this page.");
define("_GT_Section","Page");
define("_GT_Previous_Page","Previous Page");
define("_GT_Save","Save");
define("_GT_Next_Page","Next Page");
define("_GT_Submit_Survey","Submit Survey");
define("_GT_Edit_this_field","Edit Question - or choose Question Number to edit");
define("_GT_Field","Field");
define("_GT_Field_Name","Field Name");
define("_GT_Type","Type");
define("_GT_Length","Length");
define("_GT_Precision","Precision");
define("_GT_Enter_the_possible_answers","Answer Choices (if applicable)<br />For fill-in-the-blank answer, enter %s");
define("_GT_Add_another_answer_line","Additional Answer Choice");
define("_GT_Please_select_a_group","Select Group");
define("_GT_Private","Private");
define("_GT_Survey_Access","Survey Access");
define("_GT_This_lets_you_control","<p>Manage survey permissions:</p><ul><li><strong>Public</strong> surveys show on the module page and allows all users to submit responses</li><li><strong>Private</strong> surveys are restricted to specified groups (click on a SurveyName to manage)</li></ul>");
define("_GT_Note","Note");
define("_GT_You_must_use","Private surveys require %s (see <a href=\"docs/faq.php#d_handler\" target=\"_blank\">FAQ</a>)");
define("_GT_Group","Group");
define("_GT_Max_Responses","Max Responses");
define("_GT_Save_Restore","Save/Restore");
define("_GT_Back_Forward","Back/Forward");
define("_GT_Add","Add");
define("_GT_Make_Public","Make Public");
define("_GT_Make_Private","Make Private");
define("_GT_to_access_this_group","to access this group");
define("_GT_Cannot_delete_account","Cannot delete account");
define("_GT_Username_are_required","Username, Password, and Group are required.");
define("_GT_Error_adding_account","Error adding account");
define("_GT_Cannot_change_account_data","Cannot change account");
define("_GT_Account_not_found","Account not found");
define("_GT_Designer_Account_Administration","Designer Account Administration");
define("_GT_Username","Username");
define("_GT_Password","Password");
define("_GT_First_Name","First Name");
define("_GT_Last_Name","Last Name");
define("_GT_Expiration","Expiration");
define("_GT_year","year");
define("_GT_month","month");
define("_GT_day","day");
define("_GT_Disabled","Disabled");
define("_GT_Update","Update");
define("_GT_Cancel","Cancel");
define("_GT_Delete","Delete");
define("_GT_Design_Surveys","Design Surveys");
define("_GT_Change_Survey_Status","Change Survey Status");
define("_GT_Activate_End","Activate/End");
define("_GT_Export_Survey_Data","Export Survey Data");
define("_GT_Group_Editor","Group Editor");
define("_GT_may_edit","may edit <b>all</b> forms owned by this group");
define("_GT_Administer_Group_Members","Administer Group Members");
define("_GT_Administer_Group_Respondents","Administer Group Respondents");
define("_GT_Respondent_Account_Administration","Respondent Account Administration");
define("_GT_to_access_this_survey","to access this survey");
define("_GT_Error_copying_survey","Error copying survey");
define("_GT_Copy_Survey","Copy Survey");
define("_GT_Choose_a_survey","Choose a survey to copy - cloned survey will be created in edit mode and do not copy access permissions");
define("_GT_Status","Status");
define("_GT_Archived","Archived");
define("_GT_Ended","Ended");
define("_GT_Active","Active");
define("_GT_Testing","Testing");
define("_GT_Editing","Editing");
define("_GT_You_are_attempting","It is not possible both cross analyze and tabulate at the same time");
define("_GT_Only_superusers_allowed","Only superusers allowed");
define("_GT_No_survey_specified","No survey specified");
define("_GT_Manage_Web_Form_Designer","Manage Web Form Designer Accounts");
define("_GT_Click_on_a_username_to_edit","Choose a username to edit, or add a new user below");
define("_GT_disabled","disabled");
define("_GT_Add_a_new_Designer","Add New Designer");
define("_GT_Bulk_Upload_Designers","Bulk Upload Designers");
define("_GT_Invalid_survey_ID","Invalid survey ID");
define("_GT_DBF_download_not_yet","DBF download not yet implemented.");
define("_GT_The_PHP_dBase","PHP dBase Extension not installed");
define("_GT_Edit_a_Survey","Edit Survey");
define("_GT_Pick_Survey_to_Edit","Choose Survey to Edit");
define("_GT_Export_Data","Export Data");
define("_GT_Format","Format");
define("_GT_CSV","CSV");
define("_GT_download","download");
define("_GT_DBF","DBF");
define("_GT_HTML","HTML");
define("_GT_Testing_Survey","Testing Survey...");
define("_GT_SID","SID");
define("_GT_Survey_exported_as","Survey exported as:");
define("_GT_Error_exporting_survey_as","Error exporting survey as:");
define("_GT_to_access_this_form","to access this form");
define("_GT_Error_adding_group","Error adding group.");
define("_GT_Error_deleting_group","Error deleting group.");
define("_GT_Group_is_not_empty","Group is not empty.");
define("_GT_Manage_Groups","Manage Groups");
define("_GT_Description","Description");
define("_GT_Members","Members");
define("_GT_Users_guide_not_found","User Guide not found.");
define("_GT_Log_back_in","Log back in.");
define("_GT_Superuser","Superuser");
define("_GT_Choose_a_function","Choose Function");
define("_GT_Create_a_New_Survey","New Survey");
define("_GT_Edit_an_Existing_Survey","Edit Survey");
define("_GT_Test_a_Survey","Test Survey");
define("_GT_Copy_an_Existing_Survey","Copy Survey");
define("_GT_Change_the_Status_of_a_Survey","Survey Status");
define("_GT_active_end_delete","(Test, Activate, Archive, End, Purge)");
define("_GT_Change_Access_To_a_Survey","Survey Access Permissions");
define("_GT_Limit_Respondents","Public, Private, Responses, Save, Nav");
define("_GT_View_Results_from_a_Survey","Survey Results - Standard");
define("_GT_Cross_Tabulate_Survey_Results","Survey Results - Cross Tab");
define("_GT_View_a_Survey_Report","Survey Report - View Example");
define("_GT_Export_Data_to_CSV","Export Survey Data");
define("_GT_Change_Your_Password","Change Password");
define("_GT_Manage_Designer_Accounts","Manage Designer Accounts");
define("_GT_Manage_Respondent_Accounts","Manage Respondent Accounts");
define("_GT_View_the_list_of_things_still_to_do","View the list of things still to do");
define("_GT_development_goals","(development goals)");
define("_GT_View_the_User_Administrator_Guide","View phpESP User/Administrator Guide");
define("_GT_Log_out","Log out");
define("_GT_SIDS","SIDS");
define("_GT_Error","Error!");
define("_GT_You_need_to_select_at_least_two_surveys","You need to select at least two surveys!");
define("_GT_Merge_Survey_Results","Merge Survey Results");
define("_GT_Pick_Surveys_to_Merge","Pick Surveys to Merge");
define("_GT_List_of_Surveys","List of Surveys");
define("_GT_Surveys_to_Merge","Surveys to Merge");
define("_GT_Change_Password","Change Password");
define("_GT_Your_password_has_been_successfully_changed","Your password has been successfully changed.");
define("_GT_Password_not_set","Password not set, check your old password.");
define("_GT_New_passwords_do_not_match_or_are_blank","New passwords do not match or are blank.");
define("_GT_Old_Password","Old Password");
define("_GT_New_Password","New Password");
define("_GT_Confirm_New_Password","Confirm New Password");
define("_GT_Purge_Surveys","Purge Surveys");
define("_GT_This_page_is_not_directly","<strong>WARNING</strong>: Purging a servey <em>completely removes and deletes all survey information from the database forever</em>. This includes survey general information, questions, responses, results, etc.<br /><br /><strong><em>Purged Surveys are NOT RECOVERABLE</em></strong>");
define("_GT_Qs","# Q's");
define("_GT_Clear_Checkboxes","Clear Checkboxes");
define("_GT_README_not_found","README not found.");
define("_GT_Go_back_to_Report_Menu","Report Menu");
define("_GT_View_Form_Report","View Form Report");
define("_GT_Pick_Form_to_View","Choose Form to View");
define("_GT_Add_a_new_Respondent","Add New Respondent");
define("_GT_Bulk_Upload_Respondents","Bulk Upload Respondents");
define("_GT_Cross_Tabulation","Cross Tabulation");
define("_GT_Test_Survey","Test Survey");
define("_GT_Reset","Reset");
define("_GT_Cross_Tabulate","Cross Tabulate");
define("_GT_View_Survey_Results","View Survey Results");
define("_GT_Pick_Survey_to_View","Choose Survey to View");
define("_GT_Pick_Survey_to_Cross_Tabulate","Choose Survey to Cross Tabulate");
define("_GT_Respondent","Respondent");
define("_GT_Resp","Resp");
define("_GT_Can_not_set_survey_status","Can not set survey status.");
define("_GT_Survey_Status","Survey Status");
define("_GT_Test_transitions","<b>Test</b> - sets survey to test mode: allows you to perform a live test by taking the survey, and viewing the test results. Do not make changes to survey questions after switching to test mode");
define("_GT_Activate_transitions","<b>Activate</b> - sets survey Active: survey is then ready for production use and if Public will display on the module index page; this clears any testing results. No further editing of survey is allowed.");
define("_GT_End_transitions","<b>End</b> - closes and deactivates survey (re-activate in Copy Survey); no editing is allowed; no users may take the survey; results are viewable.");
define("_GT_Archive_removes","<b>Archive</b> - removes the survey. The survey is still stored in the database, but no further interaction is allowed (no reporting, downloading or otherwise). It may be reactivated in Copy Survey. You may still purge the survey.");
define("_GT_Test","Test");
define("_GT_Activate","Activate");
define("_GT_End","End");
define("_GT_Archive","Archive");
define("_GT_No_tabs_defined","No tabs defined. Please check your INI settings.");
define("_GT_Help","Help");
define("_GT_General","General");
define("_GT_Questions","Questions");
define("_GT_Order","Order");
define("_GT_Preview","Preview");
define("_GT_Finish","Finish");
define("_GT_Click_cancel_to_cancel","Click cancel to cancel this survey, or click continue to proceed to the next tab.");
define("_GT_The_survey_title_and_other","<strong>TABS</strong><br /><b>General</b> - Name, Title, and other survey definitions<br /><b>Questions</b> - Add, delete, modify questions<br /><b>Order</b> - add page breaks and change question order<br /><b>Preview</b> - Preview the survey<br /><b>Finish</b> - save changes and finish editing survey");
define("_GT_Click_here_to_open_the_Help_window","Open Help Window");
define("_GT_View_Results","View Results");
define("_GT_Pick_Survey_to_Test","Choose Survey to Test");
define("_GT_Export","Export");
define("_GT_Results","Results");
define("_GT_Todo_list_not_found","Todo list not found.");
define("_GT_An_error_Rows_that_failed","An error occurred during upload.  Rows that failed are listed below.");
define("_GT_An_error_Please_check_the_format","An error occurred during upload.  Please check the format of your text file.");
define("_GT_An_error_Please_complete_all_form_fields","An error occurred during upload.  Please complete all form fields.");
define("_GT_Upload_Account_Information","Upload Account Information");
define("_GT_All_fields_are_required","All fields are required");
define("_GT_File_Type","File Type");
define("_GT_Tab_Delimited","Tab Delimited");
define("_GT_File_to_upload","File to upload");
define("_GT_Thank_You_For_Completing_This_Survey","Thank you for completing the survey.");
define("_GT_Please_do_not_use_the_back_button","Please do not use the back button on your browser to go back. Instead, click the link below to return you to where you launched this survey.");
define("_GT_Unable_to_find_the_phpESP_directory","Unable to find the phpESP %s directory. \t\t\tPlease check %s to ensure that all paths are set correctly.");
define("_GT_Gettext_Test_Failed","%%%% Gettext Test Failed");
define("_GT_Survey_not_specified","Error processing survey: Survey not specified.");
define("_GT_Survey_is_not_active","Error processing survey: Survey is not active.");
define("_GT_Sorry_the_account_request_form_is_disabled","Sorry, the account request form is disabled.");
define("_GT_Please_complete_all_required_fields","Please complete all required fields.");
define("_GT_Passwords_do_not_match","Passwords do not match.");
define("_GT_Request_failed","Request failed, please choose a different username.");
define("_GT_Your_account_has_been_created","Your account, %s, has been created!");
define("_GT_Account_Request_Form","Account Request Form");
define("_GT_Please_complete_the_following","Please complete the following form to request an account. Items marked with a %s are required.");
define("_GT_Email_Address","Email Address");
define("_GT_Confirm_Password","Confirm Password");
?>
