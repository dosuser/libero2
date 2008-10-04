echo -n "커밋 메시지 를 입력해주세요: ";
read M;
svn ci ~/ -m "$M"
