# Phone-only cloud build — development prototype

**This is NOT a CIA and is not guaranteed to compile.** It contains a GitHub Actions build workflow to compile the existing 3DSX code using a hosted Linux runner. You do not need Windows or to install devkitPro yourself. GitHub's runner uses devkitPro remotely.

## On your iPhone
1. Sign in to GitHub and create a **private** repository (recommended because this ZIP includes extracted game assets).
2. Upload the **contents** of this folder to the repository, including `.github/workflows/build.yml`. GitHub's web interface can be awkward about uploading hidden folders; if `.github` is missing, create the file in GitHub using **Add file > Create new file**, naming it `.github/workflows/build.yml`, then paste the workflow text from this ZIP.
3. Open the repository **Actions** tab; select **Build 3DS homebrew**; choose **Run workflow**.
4. Open the resulting job and inspect the compiler output. If successful, download the `tom-ben-3dsx-unverified` artifact ZIP. It is a Homebrew Launcher `.3dsx`, **not a CIA**.
5. An `.3dsx` launch also requires the included `sdmc/3ds/tom_ben_news/` folder copied onto your 3DS SD card.

## Current gaps
- Original iOS source was not included in the supplied archive; code is a partial original-3DS homebrew recreation.
- There is no tested CIA pipeline yet; makerom and metadata generation are not set up.
- Audio events, proper chair animations, Facty sprite, microphone testing, and other actions remain unfinished.
- The build workflow was authored but **not run on GitHub** from this environment.

## Publishing
The `TomBenNews_Website.zip` from the previous package is a static website, not yet public. Avoid posting redistributed proprietary game assets and binaries without rights to do so.
