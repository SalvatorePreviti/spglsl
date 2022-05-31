import { spglslPreload } from "spglsl";
import { makeTestShader } from "./lib/test-shader";

import { getTestShaders } from "./shaders/test-shaders";

describe("angle-regression", function () {
  this.timeout(7000);

  before(async () => {
    await spglslPreload();
  });

  for (const testShader of getTestShaders()) {
    if (testShader.hasIncludes) {
      continue;
    }

    it(`ANGLE regression ./${testShader.name}`, async () => {
      return makeTestShader(testShader)();
    });
  }
});
