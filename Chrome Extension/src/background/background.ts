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
    end_lock_time: 0,
  };
  setStoredOptions(values);
  console.log(getStoredOptions());
});

chrome.tabs.onUpdated.addListener((tabId, changeInfo, tab) => {
  console.log(getStoredOptions());
  getStoredOptions().then((options) => {
    console.log(options.is_autocomplete_enabled);
    if (
      changeInfo.status === "complete" &&
      options.is_autocomplete_enabled === false
    ) {
      chrome.scripting.executeScript({
        target: { tabId: tabId },
        files: ["content.js"],
      });
    }
  });
});

// Print hello world every second
setInterval(() => {
  getStoredOptions().then((options) => {
    if (options.is_locked === false) {
      if (options.end_lock_time > Date.now()) {
        setStoredOptions({
          ...options,
          is_locked: true,
        });
      }
    } else {
      console.log("not locked");
    }
  });
}, 1000);
