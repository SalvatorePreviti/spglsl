(() => {
  window.addEventListener("error", (ev) => {
    console.error(ev.error);
    try {
      window.reportTestResultsToHarness(false, `Page error ${ev.error}`);
    } catch (e) {
      console.error(e);
    }
  });

  function begin() {
    const request = new XMLHttpRequest();
    request.open("POST", "/harness/begin", false);
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(
      JSON.stringify({
        __sp_test_file: window.__sp_test_file,
      }),
    );
  }

  const prev_reportTestResultsToHarness = window.reportTestResultsToHarness;
  window.reportTestResultsToHarness = (success, msg) => {
    if (!success) {
      console.log("err", msg);
      const request = new XMLHttpRequest();
      request.open("POST", "/harness/error", false);
      request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
      request.send(
        JSON.stringify({
          __sp_test_file: window.__sp_test_file,
          message: msg,
        }),
      );
    }
    if (prev_reportTestResultsToHarness) {
      prev_reportTestResultsToHarness(success, msg);
    }
  };

  const prev_notifyFinishedToHarness = window.notifyFinishedToHarness;
  window.notifyFinishedToHarness = () => {
    console.log("notifyFinishedToHarness");
    const request = new XMLHttpRequest();
    request.open("POST", "/harness/end", true);
    request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
    request.send(
      JSON.stringify({
        __sp_test_file: window.__sp_test_file,
      }),
    );
    if (prev_notifyFinishedToHarness) {
      prev_notifyFinishedToHarness();
    }
    if (window.__sp_automated_test_end) {
      window.__sp_automated_test_end();
    }
  };

  const _shaderDataMap = new WeakMap();

  /**
   * @param {typeof WebGL2RenderingContext | typeof WebGLRenderingContext} contextClass
   */
  function initContextProto(contextClass) {
    const proto = contextClass.prototype;

    const glGetShaderInfoLog = proto.getShaderInfoLog;
    const glCreateShader = proto.createShader;
    const glShaderSource = proto.shaderSource;

    const _compileCache = {
      vert: new Map(),
      frag: new Map(),
    };

    proto.getShaderInfoLog = function getShaderInfoLog(shader) {
      let browserInfolog = glGetShaderInfoLog.call(this, shader);
      const spglslResult = _shaderDataMap.get(shader);
      if (spglslResult && spglslResult.infolog !== undefined) {
        const existingInfolog = new Set((spglslResult.infolog || "").split("\n"));
        for (const line of (browserInfolog || "").split("\n")) {
          if (!existingInfolog.has(line)) {
            if (browserInfolog.length && !browserInfolog.endsWith("\n")) {
              browserInfolog += "\n";
            }
            browserInfolog += `${line}\n`;
          }
        }

        if (!spglslResult._logged && browserInfolog && browserInfolog !== "\n") {
          spglslResult._logged = true;
          console.log("\n----browser inflog----\n", `${browserInfolog}\n`, "\n----=============----\n");
        }
      }
      return browserInfolog;
    };

    proto.createShader = function createShader(type) {
      const result = glCreateShader.call(this, type);

      if (type === this.VERTEX_SHADER) {
        _shaderDataMap.set(result, { shaderType: "vert" });
      } else if (type === this.FRAGMENT_SHADER) {
        _shaderDataMap.set(result, { shaderType: "frag" });
      }

      return result;
    };

    proto.shaderSource = function shaderSource(shader, sourceCode) {
      const shaderData = _shaderDataMap.get(shader);
      if (!shaderData) {
        return glShaderSource.call(this, shader, sourceCode);
      }

      const shaderType = shaderData.shaderType;
      const cacheMap = _compileCache[shaderType];

      let spglslResult = cacheMap.get(sourceCode);

      if (spglslResult === undefined) {
        console.time("spglsl");
        try {
          const request = new XMLHttpRequest();
          request.open("POST", "/spglsl/glsl", false);
          request.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
          request.send(
            JSON.stringify({
              __sp_test_file: window.__sp_test_file,
              sourceCode,
              shaderType,
            }),
          );

          spglslResult = JSON.parse(request.response) || { _error: "UnknownError" };
          spglslResult.input = sourceCode;
          cacheMap.set(sourceCode, spglslResult);

          console.log(
            "----input----\n",
            `${sourceCode}\n`,
            "\n----output----\n",
            `${spglslResult.out}\n`,
            "\n----inflog----\n",
            `${spglslResult.infolog}\n`,
            "\n----=======----\n",
          );
        } catch (error) {
          console.error("spglsl", error);
          throw error;
        } finally {
          console.timeEnd("spglsl");
        }
      }

      shaderData.infolog = spglslResult.infolog;

      if (spglslResult._error) {
        const error = new Error(spglslResult.message);
        Object.defineProperty(error, "name", {
          value: spglslResult._error,
          configurable: true,
          enumerable: false,
          writable: true,
        });

        Object.defineProperty(error, "message", {
          value: spglslResult.message,
          configurable: true,
          enumerable: false,
          writable: true,
        });

        for (const key of Object.keys(spglslResult)) {
          if (!(key in error)) {
            error[key] = spglslResult[key];
          }
        }
        throw error;
      }

      if (spglslResult instanceof Error) {
        throw spglslResult;
      }

      return glShaderSource.call(this, shader, spglslResult.out);
    };
  }

  initContextProto(WebGLRenderingContext);
  initContextProto(WebGL2RenderingContext);

  begin();
})();
