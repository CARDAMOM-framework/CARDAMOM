# Using CARDAMOM's github repository


## Issues and labels

We encourage labeling all issues with one or more of the following categories:

- Framework
- MCMC
- Science Issue
  - No technical issue, but there's something wrong, suspicious or room for improvement with science algorith
- Forward model code:
  - Includes DALEC models, modules, EDCs, and Observation Operators, refactoring
- Cost function:
  - THis 
- 

## Github basics and tips

Issue "There is no tracking information for the current branch", when you try to "git push" for first time

(1) git push <cardamom> (or the name of the remote, use "git remote" to find this out). This creates the branch on "cardamom" repo.
(2)  git branch --set-upstream-to=<cardamom>/<branch> testonly. This tracks the cardamom repo branch
  
  You can then "git pull" or "git push"

  
  ###Merge changes from main into your branch
  
  git merge main
  
  ## Delete branch remotely (e.g. "testonly") and remote is "cardamom
  git push cardamom --delete testonly
  #And delete locally
  git branch -D testonly
  




https://stackoverflow.com/questions/32056324/there-is-no-tracking-information-for-the-current-branch



git push >> fatal: no configured push destination


https://stackoverflow.com/questions/10032964/git-push-fatal-no-configured-push-destination
