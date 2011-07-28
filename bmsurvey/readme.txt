--------------------------------------------------------------------------------
Module Name  : Bluemoon.Multi-Survey / Code Name: Irene
Copyright(c) : Yoshi.Sakai ( webmaster@bluemooninc.biz )
Home Page    : http://www.bluemooninc.biz/
Start date   : 21,Jun,2004
License under: GPL
--------------------------------------------------------------------------------
Based By : phpESP V1.6.1 James Flemer <jflemer@alum.rpi.edu>
--------------------------------------------------------------------------------*****************
Special Thanks to
*****************
All versions are based the phpESP by James Flemer.
V0.7 Sponsored by DO HOUSE Inc.
V0.8 Sponsored by DO HOUSE Inc.

************
Introduction
************
The Bluemoon.Multi-Survey (bmsurvey) is based on the phpESP. It has been ported to XOOPS2 with modifications
 and enhancements. You can create many different kinds of surveys.

Features:
1.Edit by Browser: You can make any kind of form without having any programming skill.
2.Easy Input: It has browser and mail questionnare interface. 
3.Get a Response: You can receive response via email with attached files and see the result by browser.
4.When combined with PopnupBlog, it can be save a response as blog (with/without attach files).

******************
System Requirement
******************
XOOPS 2.0.X
PHP gettext function. ( Use several languages with 'i18n'. )
PHP mb function. ( Currently Support English,Japanese and French. )

***************
Rapidly Install
***************
1.Extract these module files. (XOOPS_ROOT./module)
2.Install from module admin,
3.Customize module preferences at the module admin.
DONE! 

But this version is 'as is', and still going on to porting.

Happy Hacking!
Yoshi.

*************************************
Release notes. (Current versions)
*************************************
V0.80 2007/07/24 Added new 5 function.
 1.Select status at copy question from other surveys.
 2.Question number at edit questions.
 3.Count display mode at survey reports. (switch by preference at module admin)
 4.Selected number mode for radio/checkbox at CSV output. (switch by preference at module admin)
 5.Display check mark at top block when user have already responded it.

V0.80a 2007/07/27 Added check mark for survey list block.
V0.80b 2007/07/30 Bugfix for CSV no output strings as number mode. Omitted contents in copy selection box at survey edit.
V0.80c 2007/07/30 Added contents in copy selection box at survey edit.
V0.80d 2007/08/01 Question number button can alternate display when it has much numbers at survey edit.
V0.81 2007/09/06 Support MySQL after V4.1. Change password length 16 to 41 at mysql.spl. Suggested by Mario. Thanks.
V0.82 2007/12/04 Supported group permission for private survey.
V0.83 2008/01/08 Bugfix for guest goes to blank page at click survey on main menu.
V0.84 2008/02/09 Support CSV download as https protocal with IE6. Bugfix CSV check box numbering.
v0.85 2008/04/10 Update for XSS Vulnerability at download.php.
v0.86 2008/04/14 Added Label,Class to survey_render.inc(line 114,115,116,126,142,153). You can use CSS style to the radio button and check box.
v0.87 2008/07/01 Survey management has group permission.
v0.88 2008/07/20 Security update as XSS vulnerability on handler-prefix.php.
v0.89 2008/07/28 Advanced Mail send receive functions. It works with PHPfeather.
v0.90 2008/07/29 Bugfix for Mail receive function for get time strings such like "10:30".
v0.91 2008/08/05 Added numeric symbol when cast survey by email.
v0.92 2008/08/19 Permitted group can use the manage menu when xoops module management off. At "Change Access To a Survey", The group user cannot change.
v0.93 2008/08/21 Added Manage group status at module admin.
V0.94 2008/10/23 Changed on CSV download. mb_internal_encording() to _CHARSET "As XOOPS default char setting".
V0.95 2008/11/15 Supported SMTPauth,SMTP,sendmail ( using xoopsMailer )
V0.96 2008/11/26 Bugfix: In the selection item with other strings, it is included with the response mail. 
V0.97 2009/01/10 SAY BYE-BYE TO THE GetText liblary. alpha-1.

*********************************
Release notes. (Old versions)
*********************************
v0.01 2004/07/24 Alpha release.
v0.02 2004/07/25 The mail output is carried out in the same order as a display.
v0.03 2004/08/13 Move folder setting from phpESP.ini.php to config.php.
v0.04 2004/08/21 Bugfix about URL for image files.
v0.05 2004/08/24 Support for owner to Xoops group who can see survey result.
v0.06 2004/08/26 Add parameter 'csv_charset' for CSV file encording in phpESP.ini.php.
v0.07 2004/08/26 The result page was reformed.
v0.08 2004/09/14 Change email 'from' address to $xoopsUser->email who made survey. Add French by Outch.  
v0.09 2004/09/18 Bugfix for non-support about PHP mb-functions server.
v0.10 2004/10/04 Bugfix about undefined nortice $referer on handler.php.
v0.11 2004/10/11 Bugfix about status change to activate survey.
v0.12 2004/10/13 Support smarty at test survey. It works, but break a block little.
v0.13 2004/10/20 Bugfix about unvisible section text.
v0.14 2004/10/23 Omit the table width about bmsurvey_survay.html.
v0.15 2004/11/07 Bugfix about sendmail with attach file. Default language is English.
v0.16 2004/11/12 Support deny result and display user name at View Survey Results. Send result to respondent.
v0.17 2004/11/16 Reactivate survey from Ended or Archived status at Copy Survey.
v0.18 2004/11/24 Support HTML file download at Export Data to CSV.
                 Support date order by _SHORTDATESTRING in XOOPS/language/yourlanguage/global.php.
                 Support auto set your language by _CHARSET,_LANGCODE in XOOPS/language/yourlanguage/global.php
                 Convert CSV SUBMITTED date that MySQL timestamp to _DATESTRING in XOOPS/language/yourlanguage/global.php.
                 Bugfix about disapper mail message problem. It happen send to popnupBlog with attach file. 
                 Add upload file parameters in phpESP.ini.php. (subtype,imgtype,viri)
Riv.a 2004/12/16 Bugfix about it couldn't send a email with attach file.
v0.19 2004/12/18 Support send a email with multiple attached files. Mail code setting free.
v0.20 2004/12/19 Add purge menu into survey status. change '\n' to space in japanese po,mo file.
---------------> Other languages, please change '\n' to space in po file and compile it to mo.
v0.21 2005/01/15 Change a parameter mothod $ESPCONFIG['allow_email'] boolean to int. It has 5 paramters for send responce ( 0= Nothing, 1=creator to respondent, 2=respondent to creator, 3=both, 4=creator to creator )
Riv.a 2005/01/15 Add param=5 to $ESPCONFIG['allow_email'] from Creator to Site Admin (For PopnupBlog)
Riv.b 2005/02/03 Bugfix about null from address of response mail.
                 Add param=6 to $ESPCONFIG['allow_email'] from Site Admin to Creator (For PopnupBlog)
                 Add param=7 to $ESPCONFIG['allow_email'] from Creator to Special address (For PopnupBlog)
                 Add new parameter $ESPCONFIG['special_addr'] work with 'allow_email'=7.
Riv.c 2005/02/06 Bugfix about it couldn't upload file with required flag.
v0.22 2005/02/19 Support Confirmation Page (URL) or (heading text,body text).
v0.23 2005/02/20 The preview form fixed by an actual form display. And lift up an actual form.
Riv.a 2005/02/22 Bugfix about admin/test.php.
v0.24 2005/02/26 Add anew parameter $ESPCONFIG['special_char'] work with 'allow_email'=7.
- PortAngel 427  Cast-Survey project started. I named developpment code 'PortAngel'
                 Add survey_id in bmsurvey_respondent.
v0.3  2005/05/30 Cast-Survey has been done. You can send questionnaire to anyone and catch it.
- PortAngel 530  Renewal release. All name were changed BMEF to BMSURVEY.
- PortAngel 531  Bugfix about language/english/admin.php
- PortAngel 531a Bugfix about mysql.sql. I forget response_id in respondent table.
- PortAngel 601  Add a new parameter for block list number at module preference.
- PortAngel 603  Bugfix about export CSV data. I forget delete a debug message.
- PortAngel 608  Add option as include additional info to response mail.
                 Add option as send email to 'email' in questionnaire.
                 Move Survey Managemant to Admin.
v0.4  2005/06/12 Support mailto option and fill a default response for input example.
      Irene-613  Bugfix about respondent.php and update_respondent() in bmsurveyUtils.php
      Irene-614  Support test mode on survey management.
      Irene-615  Add a 'Add usage' option at preference for questionnaire mail.
                 Support dropdown list for questionnaire mail.
      Irene-616  Bugfix about from address for questionnaire mail and edit respondents.
      Irene-616a Bugfix about survey_id conflict in edit respondents.
      Irene-616b Support NTT DoCoMo mail for response.
      Irene-617  Bugfix about NTT DoCoMo mail for response.
      Irene-617a Bugfix about CSV and HTML output at 'view a survey result'.
      Irene-617b Bugfix about Cross Tabulation.
      Irene-630  Add a 'ONE_RESPONSE' parameter on preference for response limitation.
      Irene-703  Add a 'TO_MAILINGLIST' option for mailing list.
      Irene-724  Bugfix about file_charset for mb-strings with attached file.
v0.5  2005/08/10 For running with XOOPS2.2
      Irene-1004 Fill the field name if void.
      Irene-1026 Security update for image file upload.
      Irene-1116 Fix for not allow email when PHP safe mode.
v0.6  2006/01/24 Add $ESPCONFIG['anonymousname'] for record respondent name as IP Address or 'Anonymous' switch.
      2006/01/27 Add $_SESSION['bmsurvey'] for smarty $smarty.session.bmsurvey. you can handle response values as sid = Survey ID,qid = Question ID, val = Value.
      2006/01/28 Add submenu.php for customize supecial sub menu.
      2006/01/31 Support multiple choice for $_SESSION['bmsurvey'].
      2006/02/02 Bugfix for Call to undefined function: _() in order.inc.
      2006/02/03 Support rank choice for $_SESSION['bmsurvey'].
v0.61 2006/02/03 Add limitation for multiple choice as precise parameter. Add digit check for numeric answer.
      2006/02/03 Support ranking header strings as [TH]num1,num2,num3...[/TH]. It just like a bbcode set on question's text.
      2006/02/04 Add reset function for radio button. Fix for CSV warning messages. Move missing required message from locale to language.
      2006/02/05 Bugfix for upload. polish submenu.php and submenu_ja.php.
      2006/02/06 Add a Reset Radio button setting at module preference.
      2006/02/12 Bugfix for !other html tag, it will occur with reset radio button.
      2006/03/08 Bugfix for lost final questions response and upload file information at CSV output.
      2006/03/12 Bugfix for rate response at csv output. Support uncount for invalid response(mean count only complete='Y' at response table) on the crosstab. Change HTTP_GET/POST_VARS to _GET/_POST(Say bye to PHP3, hello to PHP5)
v0.62 2006/05/05 Bugfix: Still remain HTTP_GET/POST_VARS. Fix to _GET/POST.
v0.63 2006/08/25 Bugfix for mailto method in section text.
      2006/10/16 For mbstring of CSV download, cange "auto" to mb_internal_encoding() in survey_export_csv.inc.
v0.64 2006/10/29 Change HTTP_SERVER_VARS to _SERVER as support PHP5.
V0.70 2007/02/05  Add a new function as copy question from surveys list.
     Add number strings for CSV column title at module admin.
V0.71 2007/03/01 Typo '?>' fix at languages/english/admin.php, admin/index.php
     Support CSV timestamp conversion at MySQL4.1 and before.
     At admin/include/function/survey_export_csv.inc line(108).
V0.72 2007/03/02 Bugfix about doesn't save a response of rating[1..5]. Thanks, Peter.
V0.73 2007/06/20 Bugfix about doesn't sorting of choice list on CSV export.
