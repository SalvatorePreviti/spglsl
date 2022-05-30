import { inspect } from "util";

const _symIndices = Symbol("indices");

const { defineProperty } = Object;

export type StringEnum<T extends string> = {
  readonly [P in T]: P;
};

export type StringEnumValue<T extends string | StringEnum<any>> = T extends string ? T : keyof T;

// eslint-disable-next-line @typescript-eslint/no-redeclare
export function StringEnum<TArray extends readonly string[]>(...values: TArray): StringEnum<TArray[number]> {
  const array = Array.from(new Set(values)) as any;

  const base = {};
  defineProperty(base, Symbol.iterator, { value: () => array[Symbol.iterator]() });
  defineProperty(base, inspect.custom, { value: () => array });

  const indices = new Map<string, number>();
  defineProperty(base, _symIndices, { value: indices });

  const result = Object.create(base);
  for (let i = 0, len = values.length; i < len; ++i) {
    const value = values[i]!;
    defineProperty(result, value, { value, configurable: false, enumerable: true, writable: false });
    indices.set(value, i);
  }

  return result;
}

StringEnum.iterate = <T extends string>(stringEnum: StringEnum<T>) =>
  (stringEnum as unknown as Iterable<T>)[Symbol.iterator]();

StringEnum.keys = Object.keys as <T extends string>(stringEnum: StringEnum<T>) => StringEnumValue<T>[];

StringEnum.has = <T extends string>(stringEnum: StringEnum<T>, value: string | null | undefined): value is T =>
  typeof value === "string" && value in stringEnum;

StringEnum.indexOf = <T extends string>(
  stringEnum: StringEnum<T>,
  value: string | null | undefined | false,
): number => {
  const found = ((stringEnum as any)[_symIndices] || _loadStringEnumIndices(stringEnum)).get(value);
  return found !== undefined ? found : -1;
};

StringEnum.compare = <T extends string>(
  stringEnum: StringEnum<T>,
  a: string | null | undefined | false,
  b: string | null | undefined | false,
): number => {
  const ia = StringEnum.indexOf(stringEnum, a);
  const ib = StringEnum.indexOf(stringEnum, b);
  if (ia < 0) {
    if (ib < 0) {
      if ((a as any) < (b as any)) {
        return -1;
      }
      if ((a as any) > (b as any)) {
        return 1;
      }
      return 0;
    }
    return 1;
  }
  if (ib < 0) {
    return -1;
  }
  if (ia < ib) {
    return -1;
  }
  if (ia > ib) {
    return 1;
  }
  return 0;
};

StringEnum.max = <T extends string>(stringEnum: StringEnum<T>, a: string, b: string): string => {
  return StringEnum.compare(stringEnum, a, b) > 0 ? a : b;
};

function _loadStringEnumIndices(stringEnum: any) {
  const values = Object.values(stringEnum);
  const indices = new Map();
  for (let i = 0, len = values.length; i < len; ++i) {
    const v = values[i];
    if (typeof v === "string") {
      indices.set(v, i);
    }
  }
  defineProperty(stringEnum, _symIndices, { value: indices });
  return indices;
}
