# CARDAMOM user GitHub instructions
***Anthony Bloom, Gregory R Quetin, Paul Levine, Shuang Ma, and others***  
[If you’re making any edits, add your name here!]
# 1. MUST READ: 
* Always work in a separate branch, and merge back to master via pull request. Never commit directly to master unless you are able to do a conflict-free pull from the master immediately before committing.
* Do NOT make ‘unnecessary’ changes to the files outside of your MODEL ID folder, eg. change ‘paths’ in the code, as these changes does not apply to other users but will be pushed to the main line. For your own convenience, save the modified files to your local directory, eg. ‘My CARDAMOM Projects’. You shouldn’t need to change paths in any of the c/matlab code, as they use relative path, not hard coded. If you do need to edit lines outside of your MODEL ID folder, eg. add your MODEL ID to the library, make sure to test it on your own branch first, and also make sure this change will not create bugs for other users. The user manual.md file on github give guidance on occasions you make changes outside the MODEL ID folder. Ask a team member for help if you are not sure. 
* Do NOT commit changes to the master branch unless you have just pulled from the master branch to merge any subsequent changes into whatever you have. If you attempt to pull, and are told there are conflicts, then DEFINITELY DO NOT commit at that point, because those conflicts will get propagated into the master branch. Instead, you should be committing to a separate branch, and then make a pull request to merge your branch back into master, which will let you resolve any conflicts on a line-by-line basis.
# 2. Split out your own branch.
# 3. Edit your own branch until functional. (always regularly pull from master to keep your branch updated during the development)
# 4. When you’re ready to merge your changes to the master:
1) Update your branch from the main line (resolve conflicts). 
2) And then pull request to the main line (This is where we could have someone else double check, still under discussion on details of action).
# 5. Tips:
## To create a new branch from your local and then push to the remote:
1) git status # there shouldn’t be any red or green files before proceed, if any, commit them or resolve conflicts as needed
2) git checkout –b branchname # create a new branch on your local
3) git push -u origin HEAD        # this step push your new branch to remote and track it
 
## If you changed your branch name from github (repo website), you need to manually change the name locally:
1) git branch -m oldbranch newbranch
2) git fetch origin
3) git branch -u origin/ newbranch newbranch  (This step is equivalent to git push -u origin HEAD)

## To sync your local dev_branch to the latest master on remote:
1) git checkout dev_branch # make sure you are on your local dev_branch
2)Git pull origin master # It will pull master from remote to your local master, then merge to your local dev_branch

## To push my local development branch to remote development branch:
1) git checkout dev_branch # To stay on your local branch 
2) git push origin dev_branch # It pushes your local dev_branch to the remote dev_branch; ‘origin’ always means the remote repo
