
chrome.tabs.onUpdated.addListener( function (tabId, changeInfo, tab) {
  if (tab.id && changeInfo.status == 'complete' && tab.active) {

    // do your things
    chrome.tabs.executeScript({
      file: "lib/focused-element.js",
    });
  }
})
