import { startServer } from "./conformance-test-server";

startServer().catch((e) => {
  if (!process.exitCode) {
    process.exitCode = 1;
  }
  console.error(e);
});
