import {
  getStoredOptions,
  LocalStorageOptions,
  setStoredOptions,
} from "../utils/storage";

chrome.runtime.onInstalled.addListener(() => {
  const values: LocalStorageOptions = {
    is_autocomplete_enabled: true,
    lock_after_minutes: 5,
    is_locked: true,
  };
  setStoredOptions(values);
  // console.log(getStoredOptions());
});

chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  getStoredOptions().then((options) => {
    console.log(options);
    if (
      changeInfo.status === "complete" &&
      options.is_autocomplete_enabled &&
      !options.is_locked
    ) {
      chrome.scripting.executeScript({
        target: { tabId: tabId },
        files: ["content.js"],
      });
    }
  });
}
);
