import { Box } from "@mui/material";
import React, { useEffect } from "react";
import FixedBottomNavigation from "./components/main";

const App: React.FC<{}> = () => {
  return (
    <div>
      <Box sx={{maxWidth: "100%", maxHeight: "100%"}}>
        <FixedBottomNavigation />
      </Box>
    </div>
  );
};

export default App;
