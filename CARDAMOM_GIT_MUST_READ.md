# Two sections in this file:
1. CARDAMOM user GitHub instructions (New users are required to go through this section)
2. Instructions to correct an unintentional git commit and git push that bring files to an older version, or unintentionally stashed changes, committed and pushed to the master

***Shuang Ma and others***  
[If you’re making any edits, add your name here!]

# 1. CARDAMOM user GitHub instructions (New users are required to go through this section)
## 1) MUST READ: 
* Always work in a separate branch, and merge back to master via pull request. Never commit directly to master unless you are able to do a conflict-free pull from the master immediately before committing.
* Do NOT make ‘unnecessary’ changes to the files outside of your MODEL ID folder, eg. change ‘paths’ in the code, as these changes does not apply to other users but will be pushed to the main line. For your own convenience, save the modified files to your local directory, eg. ‘My CARDAMOM Projects’. You shouldn’t need to change paths in any of the c/matlab code, as they use relative path, not hard coded. If you do need to edit lines outside of your MODEL ID folder, eg. add your MODEL ID to the library, make sure to test it on your own branch first, and also make sure this change will not create bugs for other users. The user manual.md file on github give guidance on occasions you make changes outside the MODEL ID folder. Ask a team member for help if you are not sure. 
* Do NOT commit changes to the master branch unless you have just pulled from the master branch to merge any subsequent changes into whatever you have. If you attempt to pull, and are told there are conflicts, then DEFINITELY DO NOT commit at that point, because those conflicts will get propagated into the master branch. Instead, you should be committing to a separate branch, and then make a pull request to merge your branch back into master, which will let you resolve any conflicts on a line-by-line basis.
## 2) Split out your own branch.
## 3) Edit your own branch until functional. (always regularly pull from master to keep your branch updated during the development)
## 4) When you’re ready to merge your changes to the master:
1) Update your branch from the main line (resolve conflicts). 
2) And then pull request to the main line (This is where we could have someone else double check, still under discussion on details of action).
## 5) Tips:
### To create a new branch from your local and then push to the remote:
1) git status # there shouldn’t be any red or green files before proceed, if any, commit them or resolve conflicts as needed
2) git checkout –b branchname # create a new branch on your local
3) git push -u origin HEAD        # this step push your new branch to remote and track it
 
### If you changed your branch name from github (repo website), you need to manually change the name locally:
1) git branch -m oldbranch newbranch
2) git fetch origin
3) git branch -u origin/ newbranch newbranch  # This step is equivalent to git push -u origin HEAD

### To sync your local dev_branch to the latest master on remote:
1) git checkout dev_branch # make sure you are on your local dev_branch
2) git pull origin master # It will pull master from remote to your local master, then merge to your local dev_branch

### To push your local development branch to remote development branch:
1) git checkout dev_branch # To stay on your local branch 
2) git push origin dev_branch # It pushes your local dev_branch to the remote dev_branch; ‘origin’ always means the remote repo

# 2. Instructions to correct an unintentional git commit and git push that bring files to an older version, or unintentionally stashed changes, committed and pushed to the master

0. If you need help to identify the problem. Let other CARDAMOM developers be aware of this issue and specify the problem. 
1. Find the commit ID that caused the problems (eg. the ID looks like ‘222fb39c3d39e70904371a63b14a346cfb29db08’)
2. To be safe, we encourage you to do this fix on a new branch, then pull request to merge after all looks good. Eg. create a new branch called ‘commit_revert_demo_April2021’
3. On the branch ‘commit_revert_demo_April2021’, and run: git revert 222fb39c3d39e70904371a63b14a346cfb29db08 
FYI: This is just an example, please remember to replace the commit ID 
Git revert basically creates a new commit at the head of your current branch that is the inverse of the reverted commit. Basically a selective ‘undo’. For users, when they go to push to master next time, they will have to pull this revert commit into their tree. Basically, reverting in git is seen like any other action, and it shows up as a commit in your history. You can use git revert to ‘undo’ either a historical commit or a latest commit that you want to call back. It will not revert changes made after a historical commit.
5. On the ‘commit_revert_demo_April2021’, double check 1) if the unintentionally changes files are changed back, and 2) if the later commits after ‘222fb39c3d39e70904371a63b14a346cfb29db08’ are kept; 3) if the later commits happen to have conflicts, manually resolve the conflicts. In theory step 1) and 2) should always be successful.
6. Now that you double checked all the files are correct on branch ‘commit_revert_demo_April2021’, you can do pull request and merge this branch in to master.
7. All users, whether or not you’ve made changes after the reverted commit, save your changes with ‘git add’, then pull from the master. 
More useful tips from Alex Patton: ‘If you have made changes to your local repository and are not up to date with the origin master branch, then doing git a git pull will attempt to auto-merge the changes from master into your local version when you run git pull. If git discovers that you have made changes that are incompatible with the changes on origin master branch, then it will alert you to a merge conflict, and you will have to go into your code and sort out the conflicts yourself. Git will let you know which files it can’t merge if that happens. (Note that this paragraph is how git always works with regard to commits, and is not unique to the git revert I did)
8. As noted in the user instructions, work on your own branch is the best way to avoid dealing with this kind of problems.
