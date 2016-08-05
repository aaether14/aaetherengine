#GIT PUSH MACRO by Aaether


	printf "Adding all files in the repository!\n"
	git add --all
	printf "Commiting chagnes!\n"
	git commit -m "$1"
	printf "Pushing commit to remote source!\n"
	git push
