import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "../utils/storage";

// When the extension is installed for the first time the default options are stored in local storage
chrome.runtime.onInstalled.addListener(() => {
  const values: LocalStorageOptions = {
    is_autocomplete_enabled: true,
    lock_after_minutes: 5,
    is_locked: true,
  };
  setStoredOptions(values);
  // console.log(getStoredOptions());
});

// When a tab of the chrome browser is updated (loaded), the extension execute the script for the autocompletion
chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  getStoredOptions().then((options) => {
    console.log(options);
    if (
      changeInfo.status === "complete"
    ) {
      chrome.scripting.executeScript({
        target: { tabId: tabId },
        files: ["content.js"],
      });
    }
  });
}
);
