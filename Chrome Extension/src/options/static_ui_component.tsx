import React from "react";
import {
  Card,
  CardContent,
  Typography,
} from "@mui/material";

export function OptionsPageHeader() {
  return (
    <Card>
      <CardContent>
        <Typography variant="h5" component="h2">
          Options
        </Typography>
      </CardContent>
    </Card>
  );
}