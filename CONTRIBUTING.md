# Contributing to Web Hero

This is a 3-person team project (Anha, Nazifa, Chand). This file explains
how we use git/GitHub together so nobody overwrites anyone else's work.

If you're new to git, read this whole file once before your first commit.
It's short and will save you real pain later.

## The branches

- `main` — the working, tested version of the game. **Nobody edits this
  directly.** It only changes when a finished feature is merged in.
- `feature/anha`, `feature/nazifa`, `feature/chand` — each person's own
  workspace. Work here freely. Nothing you do on your branch affects
  anyone else until you merge it.

## Daily workflow

Every time you sit down to work:

```bash
git checkout feature/yourname     # make sure you're on YOUR branch
git pull                          # get any updates to your branch
```

While working, check what's changed:
```bash
git status
```

When you've made progress you want to save:
```bash
git add .
git commit -m "short description of what you did"
git push
```

Commit often — small commits with clear messages ("added jump sound",
"fixed enemy collision bug") are much easier to deal with than one giant
commit at the end. There's no such thing as committing "too much."

## Getting your work into the real game (main)

When a feature actually works and you want it included in the main game:

1. Go to the repo on GitHub.com
2. Click the **Pull requests** tab
3. Click **New pull request**
4. Set base = `main`, compare = `feature/yourname`
5. Click **Create pull request**, write a short description of what you built
6. Click **Merge pull request**

Do this in small chunks (one feature/fix at a time) rather than one huge
merge at the end of the project. It's much easier to spot problems in a
small pull request than a massive one.

## Important warnings

**Always check which branch you're on before doing anything.**
```bash
git branch
```
The branch with the `*` next to it is the one you're currently on. If
you're not sure, run this before every work session. Committing to the
wrong branch by accident is the single most common git mistake.

**Never edit directly on `main`.** If you accidentally start working on
`main`, stop, and ask before committing — don't push it. We can fix it,
but it's easier to fix before a push than after.

**Pull before you push.** If `git push` is rejected with a message about
the remote containing work you don't have, run `git pull` first, resolve
anything it asks you to, then push again. Don't force-push (`git push -f`)
unless everyone agrees first — it can permanently delete a teammate's work.

**If you see "CONFLICT" after a pull or merge:** stop and don't guess.
Take a screenshot of the terminal output and ask the team or Claude
before doing anything else. Conflicts are normal and fixable, but
guessing at a fix can make it worse.

**Try not to all three edit the exact same file at the exact same time.**
We've structured the code so each game system (hero, web-swing, enemies,
levels) lives in its own pair of files. Pick different systems to work on
at the same time and conflicts will be rare.

**Before you start a new feature, make sure your branch is up to date
with `main`**, so you're not building on outdated code:
```bash
git checkout feature/yourname
git merge main
```
Do this especially after someone else's pull request gets merged.

## Project structure reminder

See `README.md` in this folder for the file/folder layout and how the
asset-swapping system works.
