<?php
// $Id: main.php,v 1.1.1.1 2005/08/10 12:14:04 yoshis Exp $
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

define('_MD_BMSURVEY_LIST_CHECKED', '済');
define('_MD_BMSURVEY_LIST_TITLE', 'フォーム名');
define('_MD_FROM_OPTION','回答結果を送信する時の差出人アドレス（サーベイのアドレスはモジュール管理の一般設定で設定します）');
define('_MD_FROM_OPTION_0','サーベイのアドレス');
define('_MD_FROM_OPTION_1','登録ユーザのアドレス');
define('_MD_FROM_OPTION_2',"アンケート中の'email'欄");
define('_MD_FROM_DEFRES',"入力初期値のレスポンスID（初期値不要の場合は空欄にします）");
define('_MD_BMSURVEY_LIST_UNAME', '作成者');
define('_MD_BMSURVEY_LIST_DATE', '登録日付');
define('_MD_BMSURVEY_LIST_SUBTITLE', '備考');
define('_MD_BMSURVEY_THANKS_ENTRY', 'フォームへの記入ありがとうございました。');
define('_MD_BMSURVEY_CAN_WRITE_USER_ONLY', 'ゲストユーザはフォームを編集することはできません。');
define('_MD_ASTERISK_REQUIRED', 'アスタリスク（<font color="#FF0000">＊</font>）付の項目は入力必須項目です。');
define('_MD_MISSING_REQUIRED', '以下の項目が記入されていません：');
define('_MD_MAIL_TITLE', '入力フォーム：');
define('_MD_DENYRESULT','この投稿を無効にする');
define('_MD_DENYRESULTSURE','この投稿を無効にします。よろしいですか？');
define('_MD_DENYRESULTDONE','この投稿を無効にしました。');
define('_MD_DEFAULTRESULT','この投稿を入力初期値にセットする');
define('_MD_DEFAULTRESULTDONE','この投稿を入力初期値にセットしました。');
define('_MD_RESPONDENT','投稿者名');
define('_MD_QUESTION_OTHER','その他');
define('_MD_BMSURVEY_FORMATERR', ' は正しく入力されていません');
define('_MD_BMSURVEY_DIGITERR', ' は半角で数字を入力してください');
define('_MD_BMSURVEY_MAXOVER', 'は、%u項目以上チェックできません');
define('_MD_BMSURVEY_CHECKANY', '（複数選択可）');
define('_MD_BMSURVEY_CHECKLIMIT', '（%uつまで選択可）');
define('_MD_BMSURVEY_CHECKRESET', '選択解除');
define('_MD_SUBMIT_SURVEY', '送信');
define('_MD_NEXT_PAGE', '次ページ');

define('_MD_POP_KEY_M','メンバー');
define('_MD_POP_KEY_U','使い方');
define('_MD_POP_KEY_Q','アンケート');
define('_MD_POP_KEY_ERR','POP-Key Error');
define('_MD_POP_CMD_NEW','新規登録');
define('_MD_POP_CMD_INP','回答');
define('_MD_POP_CMD_DEL','削除');
define('_MD_POP_MNEW_ENTRY','ログイン名 %s でユーザ登録しました。');
define('_MD_POP_MNEW_AREADY','そのログイン名は既に登録されています。別の名前で登録してください。');
define('_MD_POP_QINP_HEADER','返信メールを作成し、[]または()の中に入力して送信下さい。
1行に複数ある[]()はチェック項目です。[]は複数、()は1つだけ任意の1文字を入力ください。
1行に1つだけの[]はテキスト入力項目です。文字列を入力下さい。
----

');
define('_MD_POP_QINP_FAILEDLOGIN','ユーザ名か認証コードが違います。');
define('_MD_POP_QINP_SUCCEEDED','%s さんの回答を登録しました。');
define('_MD_POP_QINP_DELETEIT','このアンケートは既に回答済みです。このメールに返信すると消去できます。');
define('_MD_POP_QDEL_SUCCEEDED','%s さんの回答を削除しました。');
?>