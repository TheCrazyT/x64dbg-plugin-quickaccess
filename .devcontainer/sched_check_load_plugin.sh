apt update
apt install -y curl
RELEASE_INFO=$(curl -s -L -H "Accept: application/vnd.github+json" -H "X-GitHub-Api-Version: 2022-11-28"   https://api.github.com/repos/x64dbg/x64dbg/releases)
BLOCK=$(echo "${RELEASE_INFO}"|grep -P -A8 'tag_name.+snapshot"')
RELEASE_DATE=$(echo "${BLOCK}"|grep published_at|grep -Po '(?!")\d{4}-\d{2}-\d{2}')
FILE_NAME=$(echo "${BLOCK}"|grep -Po "snapshot.+.zip")
DOWNLOAD_URL=https://github.com/x64dbg/x64dbg/releases/download/snapshot/${FILE_NAME}
CURRENT_DATE=$(date +"%Y-%d-%m")
if [ "${RELEASE_DATE}" == $(date +"%Y-%d-%m") ] || [ "${DO_EXEC}" == "1" ]
then
    apt install -y xvfb
    Xvfb :0 -screen 0 1024x768x16 &
    cd /tmp
    wget ${DOWNLOAD_URL} && unzip ${FILE_NAME}
    RELEASE_INFO2=$(curl -s -L -H "Accept: application/vnd.github+json" -H "X-GitHub-Api-Version: 2022-11-28"   https://api.github.com/repos/TheCrazyT/x64dbg-plugin-quickaccess/releases)
    BLOCK2=$(echo "${RELEASE_INFO2}"|grep browser_download_url|grep quickaccess.dp64|head -n 1)
    DOWNLOAD_URL2=$(echo "${BLOCK2}"|grep -Po "https.+dp64")
    wget ${DOWNLOAD_URL2}
    mkdir /tmp/release/x64/plugins
    cp /tmp/quickaccess.dp64 /tmp/release/x64/plugins/
    DISPLAY=:0.0 WINEDEBUG=+loaddll timeout 10 wine /tmp/release/x64/x64dbg.exe 2>err.log
    grep quickaccess.dp64 err.log
    if [ $? == 0]
    then
        exit 0
    fi
    exit 1
fi
exit 0