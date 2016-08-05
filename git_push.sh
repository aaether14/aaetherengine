#GIT PUSH MACRO by Aaether


	printf "Adding all files in the repository!\n"
	git add .
	printf "Commiting chagnes!\n"
	git commit -m "$1"
	printf "Pushing commit to outside source!\n"
	git push
