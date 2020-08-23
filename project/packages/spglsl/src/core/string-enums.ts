import { inspect } from 'util'

const _symIndices = Symbol('indices')

const { defineProperty, getOwnPropertyNames, getOwnPropertyDescriptor } = Object

export type StringEnumValue<T extends string | StringEnum<any>> = T extends string ? T : keyof T

export type StringEnum<T extends string> = {
  readonly [P in T]: P
} & {
  readonly [Symbol.iterator]: () => IterableIterator<T>
}

export function StringEnum<TArray extends readonly string[]>(...values: TArray): StringEnum<TArray[number]> {
  const array = Array.from(new Set(values)) as any

  const base = {}
  defineProperty(base, Symbol.iterator, { value: () => array[Symbol.iterator]() })
  defineProperty(base, inspect.custom, { value: () => array })

  const indices = new Map<string, number>()
  defineProperty(base, _symIndices, { value: indices })

  const result = Object.create(base)
  for (let i = 0, len = values.length; i < len; ++i) {
    const value = values[i]
    defineProperty(result, value, { value, configurable: false, enumerable: true, writable: false })
    indices.set(value, i)
  }

  return result
}

StringEnum.keys = Object.keys as <T extends string>(stringEnum: StringEnum<T>) => StringEnumValue<T>[]

StringEnum.has = <T extends string>(stringEnum: StringEnum<T>, value: string | null | undefined): value is T =>
  typeof value === 'string' && value in stringEnum

StringEnum.indexOf = <T extends string>(
  stringEnum: StringEnum<T>,
  value: string | null | undefined | false
): number => {
  const found = ((stringEnum as any)[_symIndices] || _loadStringEnumIndices(stringEnum)).get(value)
  return found !== undefined ? found : -1
}

StringEnum.compare = <T extends string>(
  stringEnum: StringEnum<T>,
  a: string | null | undefined | false,
  b: string | null | undefined | false
): number => {
  const ia = StringEnum.indexOf(stringEnum, a)
  const ib = StringEnum.indexOf(stringEnum, b)
  if (ia < 0) {
    if (ib < 0) {
      if ((a as any) < (b as any)) {
        return -1
      }
      if ((a as any) > (b as any)) {
        return 1
      }
      return 0
    }
    return 1
  }
  if (ib < 0) {
    return -1
  }
  if (ia < ib) {
    return -1
  }
  if (ia > ib) {
    return 1
  }
  return 0
}

StringEnum.max = <T extends string>(stringEnum: StringEnum<T>, a: string, b: string): string => {
  return StringEnum.compare(stringEnum, a, b) > 0 ? a : b
}

function _loadStringEnumIndices(stringEnum: any) {
  const values = Object.values(stringEnum)
  const indices = new Map()
  for (let i = 0, len = values.length; i < len; ++i) {
    const v = values[i]
    if (typeof v === 'string') {
      indices.set(v, i)
    }
  }
  defineProperty(stringEnum, _symIndices, { value: indices })
  return indices
}

export function makeObjectReadonly<TInstance extends object>(instance: TInstance): TInstance {
  for (const key of getOwnPropertyNames(instance)) {
    if (typeof key === 'string') {
      const prop = getOwnPropertyDescriptor(instance, key)
      if (prop && prop.configurable && prop.writable) {
        defineProperty(instance, key, { ...prop, writable: false })
      }
    }
  }
  return instance
}

export function inspectClassProperties(instance: object) {
  const nset = new Set<string>()
  nset.add('constructor')

  const proto = Object.getPrototypeOf(instance)
  const className = proto && proto.constructor && proto.constructor.name

  let result: any
  if (className) {
    class FakeClass {}

    defineProperty(FakeClass, 'name', { value: Object.getPrototypeOf(instance).constructor.name })
    result = new FakeClass()
  } else {
    result = {}
  }

  for (let p = instance; p && p !== Object.prototype; p = Object.getPrototypeOf(p)) {
    const names = Object.getOwnPropertyNames(instance)
    for (const name of names) {
      if (typeof name === 'string' && !nset.has(name) && !name.startsWith('_') && !name.startsWith('#')) {
        const prop = Object.getOwnPropertyDescriptor(p, name)
        if (prop) {
          nset.add(name)
          if (prop.get) {
            result[name] = (instance as Readonly<Record<string, any>>)[name]
          } else if (!prop.set) {
            result[name] = prop.value
          }
        }
      }
    }
  }
  return result
}
