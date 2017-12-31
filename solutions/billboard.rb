#!/usr/bin/env ruby
# encoding: ascii-8bit

require 'expect'
require 'socket'
require 'pty'
require 'io/console'
require 'colorize'


def log data
    $stdout.puts data.inspect.black.on_white
end

def logerror e
    $stdout.puts e.inspect.black.on_red
end


module IOExtension
    def interact
        begin
            until self.closed? do
                readable, _, _, _ = IO.select([self, $stdin])
                readable.each do |source|
                    case source
                    when self
                        input = self.readchar
                        $stdout.print input.cyan
                    when $stdin
                        self.print $stdin.gets
                    else
                        raise StandardError
                    end
                end
            end
        rescue EOFError => e
            logerror e
            self.close
            return
        rescue Interrupt
            return
        end
    end
end

class IO
    prepend IOExtension
end


target = :local
if ARGV[0] == 'remote' then target = :remote end

case target
when :local
    $s, slave = PTY.open
    PTY.spawn(ARGV[0], :in=>slave, :out=>slave)
    $s.echo = false
when :remote
    host = ARGV[1]
    port = ARGV[2]
    $s = TCPSocket.new host, port
end

def set_text t
    puts t
    $s.puts t
    print $s.expect("> ")[0].cyan
end

def devmode
    puts "devmode"
    $s.puts "devmode"
end

print $s.expect("> ")[0].cyan
set_text "set_text " + "A" * 257
devmode

$s.interact
