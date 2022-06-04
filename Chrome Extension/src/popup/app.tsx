import { Box } from "@mui/material";
import React, { useEffect, useState } from "react";
import { getStoredOptions } from "../utils/storage";
import FixedBottomNavigation from "./components/main";

const App: React.FC<{}> = () => {
  const [pageHostname, setPageHostname] = useState("");
  const [stateValue, setStateValue] = useState(-1);

  useEffect(() => {
    chrome.tabs.query({ active: true, lastFocusedWindow: true }, (tabs) => {
      var full_url = tabs[0].url;
      // Extract the hostname from the URL
      var page_hostname = full_url.split("/")[2];
      // remove www. from the string
      page_hostname = page_hostname.replace("www.", "");
      setPageHostname(page_hostname);
    });
  }, []);

  useEffect(() => {
    getStoredOptions().then((options) => {
      console.log("app options", options);
      if (options.is_locked) {
        console.log("return: true");
        setStateValue(4)
        console.log("stateValue:", stateValue);
      } else {
        setStateValue(5)
        console.log("stateValue:", stateValue);
        console.log("return: false");
      }
    });
  }, []);

  return (
    <div>
      <Box sx={{ maxWidth: "100%", maxHeight: "100%" }}>
        {
          <FixedBottomNavigation
            hostname={pageHostname}
            default_state={stateValue}
          />
        }
      </Box>
    </div>
  );
};

export default App;
